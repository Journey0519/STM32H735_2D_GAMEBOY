/*
 * network.c
 *
 *  Created on: Jun 1, 2026
 *      Author: PC
 */
#include "network.h"
#include "math.h"

#define INPUT_SIZE (1)
#define PKT_SYNC_INIT_SIZE (1979)
#define PKT_MSG_SIZE (4)
#define TX_SIZE (4096)
#define RX_SIZE (4096)
#define PKT_SYNC_SIZE (369)

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart9;
extern input_t input_other;
extern player_t players[2];
extern obstacle_t obstacles[OBSTACLES_NUM];
extern flying_t flyings[2];
extern item_t items[TOTAL_ITEM_COUNT];
extern bullet_t bullets[30];
extern const texture_t item_textures[3];
uint8_t rx_buffer[RX_SIZE]__attribute__((aligned(32), section(".RAM_D2 ")));
uint8_t tx_buffer[TX_SIZE] __attribute__((aligned(32), section(".RAM_D2 ")));
uint8_t tx_input_buffer[INPUT_SIZE] __attribute__((aligned(32), section(".RAM_D2 ")));

uint8_t sync_buffer[RX_SIZE]__attribute__((aligned(32), section(".RAM_D2 ")));

static volatile uint8_t sync_flag = 0; //2p가 사용
static volatile uint8_t sync_init_flag = 0;

static volatile uint8_t hello_flag = 0; //둘다 1p가 사용할듯
static volatile uint8_t thank_flag = 0;


void trans_input() {
	input_t input = get_my_input();
	HAL_UART_Transmit(&huart9, (uint8_t*)&input, INPUT_SIZE, 100);
}

void trans_hello_2p() {
	if (huart1.gState != HAL_UART_STATE_READY) {
		return;
	}

	pkt_msg_t* msg = (pkt_msg_t*)tx_buffer;
	msg->head0 = 0xAA;
	msg->head1 = 0x55;
	msg->msg = HELLO;
	msg->tail = 0xEE;

	SCB_CleanDCache_by_Addr((uint32_t*)tx_buffer, PKT_MSG_SIZE);
	HAL_UART_Transmit_DMA(&huart1, tx_buffer, PKT_MSG_SIZE);

	//HAL_Delay(1000);
}

void trans_thank_2p() {
	if (huart1.gState != HAL_UART_STATE_READY) {
		return;
	}

	pkt_msg_t* msg = (pkt_msg_t*)tx_buffer;
	msg->head0 = 0xAA;
	msg->head1 = 0x55;
	msg->msg = THANK_YOU;
	msg->tail = 0xEE;

	SCB_CleanDCache_by_Addr((uint32_t*)tx_buffer, PKT_MSG_SIZE);
	HAL_UART_Transmit_DMA(&huart1, tx_buffer, PKT_MSG_SIZE);
}

void trans_sync_init_1p() {
	if (huart1.gState != HAL_UART_STATE_READY) {
		return;
	}
	pkt_sync_init_t* sync = (pkt_sync_init_t*)tx_buffer;
	sync->head0 = 0xAA;
	sync->head1 = 0x55;
	sync->tail = 0xEE;

	sync->player0 = players[0];
	sync->player1 = players[1];

	memcpy(sync->obstacles, obstacles, sizeof(obstacles));
	memcpy(sync->flyings, flyings, sizeof(flyings));
	memcpy(sync->items, items, sizeof(items));
	memcpy(sync->bullets, bullets, sizeof(bullets));

	SCB_CleanDCache_by_Addr((uint32_t*)tx_buffer, sizeof(pkt_sync_init_t));
	HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(&huart1, tx_buffer, sizeof(pkt_sync_init_t)); //size: 1979

	result = 0;
}

void trans_sync_1p() {
	if (huart1.gState != HAL_UART_STATE_READY) {
		return;
	}

	tx_buffer[0] = 0xAA;
	tx_buffer[1] = 0x55;

    uint8_t* p_buffer = tx_buffer + 2;

    for (int i = 0; i < 2; i++) {
        pkt_player_t* player = p_buffer;
        player->type = 0;
        player->index = i;
        player->active = players[i].active;
        player->x = players[i].object.position_x;
        player->y = players[i].object.position_y;
        player->health = players[i].health.health;
        player->score = players[i].score;
        p_buffer += sizeof(pkt_player_t);
    }

    for (int i = 0; i < OBSTACLES_NUM; i++) {
    	pkt_obstacle_t* obstacle = p_buffer;
    	obstacle->type = 1;
    	obstacle->index = i;
    	obstacle->active = obstacles[i].active;
    	obstacle->x = obstacles[i].object.position_x;
    	obstacle->y = obstacles[i].object.position_y;
    	obstacle->health = obstacles[i].health;
    	 p_buffer += sizeof(pkt_obstacle_t);
    }

    for (int i = 0; i < 2; i++) {
    	pkt_flying_t* flying = p_buffer;
    	flying->type = 2;
    	flying->index = i;
    	flying->active = flyings[i].active;
    	flying->x = flyings[i].obj.position_x;
    	flying->y = flyings[i].obj.position_y;
    	flying->health = flyings[i].health;
    	 p_buffer += sizeof(pkt_flying_t);
    }

    for (int i = 0; i < TOTAL_ITEM_COUNT; i++) {
    	pkt_item_t* item = p_buffer;
    	item->type = 3;
    	item->index = i;
    	item->active = items[i].active;
    	item->x = items[i].object.position_x;
    	item->y = items[i].object.position_y;
    	item->item_type = items[i].type;
    	 p_buffer += sizeof(pkt_item_t);
    }

    //end
    *p_buffer++ = 0xEE;

    uint16_t pkt_size = p_buffer - tx_buffer;

    SCB_CleanDCache_by_Addr((uint32_t*)tx_buffer, pkt_size);
    HAL_UART_Transmit_DMA(&huart1, tx_buffer, pkt_size); //size:369
}

void receive_init() {
	HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&huart9, (uint8_t*)&input_other, INPUT_SIZE);
	HAL_StatusTypeDef dbg = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, RX_SIZE);
	__HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
	//HAL_StatusTypeDef uart1 = HAL_UART_Receive_DMA(&huart1, rx_buffer, PKT_SYNC_INIT_SIZE);
}



void sync_init_2p() {
	if (sync_buffer[0] != 0xAA || sync_buffer[1] != 0x55 || sync_buffer[PKT_SYNC_INIT_SIZE - 1] != 0xEE) {
		return;
	}

	pkt_sync_init_t sync = *(pkt_sync_init_t*)sync_buffer;
	players[0] = sync.player0;
	players[1] = sync.player1;

	memcpy(obstacles, sync.obstacles, sizeof(obstacles));
	memcpy(flyings, sync.flyings, sizeof(flyings));
	memcpy(items, sync.items, sizeof(items));
	memcpy(bullets, sync.bullets, sizeof(bullets));
}

void sync_2p() {
	if (sync_buffer[0] != 0xAA || sync_buffer[1] != 0x55 || sync_buffer[PKT_SYNC_SIZE - 1] != 0xEE) {
		return;
	}

	uint8_t* p_buffer = sync_buffer;
	int index = 0;

	p_buffer += 2;

	while(*p_buffer != 0xEE) { //무조건 앞에는 플레이어 2개. 중간에 0xEE있으면 어쩌죠? -> 중간을 안읽음. 무조건 type(= 0, 1) 아니면 tail만 읽게됌.
		switch(*p_buffer) {
		case PLAYER_PKT: //player
			pkt_player_t* pkt_player =  p_buffer;
			index = pkt_player->index;

			players[index].active = pkt_player->active;
			players[index].object.position_x += (pkt_player->x - players[index].object.position_x) * 0.1f;
			players[index].object.position_y += (pkt_player->y - players[index].object.position_y) * 0.1f;
			players[index].health.health = pkt_player->health;
			players[index].score = pkt_player->score;

			p_buffer += sizeof(pkt_player_t);
			break;
		case OBSTACLE__PKT:
			pkt_obstacle_t* pkt_obstacle =  p_buffer;
			index = pkt_obstacle->index;

			obstacles[index].active = pkt_obstacle->active;
			obstacles[index].object.position_x = pkt_obstacle->x;
			obstacles[index].object.position_y = pkt_obstacle->y;
			obstacles[index].health = pkt_obstacle->health;
			p_buffer += sizeof(pkt_obstacle_t);
			break;
		case FLYING_PKT:
			pkt_flying_t* pkt_flying =  p_buffer;
			index = pkt_flying->index;

			flyings[index].active = pkt_flying->active;

			if (abs(pkt_flying->x - flyings[index].obj.position_x) > 500 || abs(pkt_flying->y - flyings[index].obj.position_y) > 500) {
				flyings[index].obj.position_x = pkt_flying->x;
				flyings[index].obj.position_y = pkt_flying->y;
			} else {
				flyings[index].obj.position_x += (pkt_flying->x - flyings[index].obj.position_x) * 0.1f;
				flyings[index].obj.position_y += (pkt_flying->y - flyings[index].obj.position_y) * 0.1f;
			}

			flyings[index].health = pkt_flying->health;
			p_buffer += sizeof(pkt_flying_t);
			break;
		case ITEM_PKT:
			pkt_item_t* pkt_item =  p_buffer;
			index = pkt_item->index;

			items[index].active = pkt_item->active;
			items[index].object.position_x = pkt_item->x;
			items[index].object.position_y = pkt_item->y;

			if (items[index].type != pkt_item->item_type) {
				items[index].item_text = item_textures[pkt_item->item_type];
			}

			items[index].type = pkt_item->item_type;
			p_buffer += sizeof(pkt_item_t);
			break;
		default:
			assert(0);
		}
	}
}

uint8_t get_thank_flag() {
	return thank_flag;
}
void set_thank_flag_zero() {
	thank_flag = 0;
}


uint8_t get_hello_flag() {
	return hello_flag;
}
void set_hello_flag_zero() {
	hello_flag = 0;
}

uint8_t get_sync_flag() {
	return sync_flag;
}
void set_sync_flag_zero() {
	sync_flag = 0;
}

uint8_t get_sync_init_flag() {
	return sync_init_flag;
}
void set_sync_init_flag_zero() {
	sync_init_flag = 0;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	SCB_InvalidateDCache_by_Addr((uint32_t *)rx_buffer, RX_SIZE);

    if (huart->Instance == USART1) {
        if (Size == PKT_SYNC_INIT_SIZE && rx_buffer[0] == 0xAA && rx_buffer[1] == 0x55 && rx_buffer[PKT_SYNC_INIT_SIZE - 1] == 0xEE) {
        	sync_init_flag = 1;
        	memcpy(sync_buffer, rx_buffer, PKT_SYNC_INIT_SIZE);
        } else if (Size == 4 && rx_buffer[0] == 0xAA && rx_buffer[1] == 0x55 && rx_buffer[3] == 0xEE) {
        	switch (rx_buffer[2]) {
        	case HELLO:
        		hello_flag = 1;
        		break;
        	case THANK_YOU:
        		thank_flag = 1;
        		break;
        	default:
				assert(0);
        	}
        } else if (rx_buffer[0] == 0xAA && rx_buffer[1] == 0x55 && rx_buffer[Size - 1] == 0xEE) {
        	sync_flag = 1;
        	memcpy(sync_buffer, rx_buffer, Size);
        } else {
        	__HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);
        }


    	huart->RxState = HAL_UART_STATE_READY;
    	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, RX_SIZE);
    	__HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        uint32_t error_code = HAL_UART_GetError(huart);

        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
        HAL_UART_AbortReceive(huart);

        huart1.ErrorCode = HAL_UART_ERROR_NONE;
        huart1.RxState = HAL_UART_STATE_READY;

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, RX_SIZE);
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
        return;
    }

    if (huart->Instance == UART9) {
    	uint32_t error_code = HAL_UART_GetError(huart);
    	__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF);

    	huart->ErrorCode = HAL_UART_ERROR_NONE;
    	huart->RxState = HAL_UART_STATE_READY;

    	HAL_UART_Receive_DMA(huart, (uint8_t*)&input_other, 1);
    	return;
    }
}
