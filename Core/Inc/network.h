/*
 * network.h
 *
 *  Created on: Jun 1, 2026
 *      Author: PC
 */

#ifndef INC_NETWORK_H_
#define INC_NETWORK_H_

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "input.h"
#include "player.h"
#include "object.h"
#include "bullet.h"
#include "obstacle.h"
#include "flying.h"
#include "item.h"

enum {
	HELLO = 0,
	//OK_TRANSMIT,
	THANK_YOU
};

enum {
	PLAYER_PKT = 0,
	OBSTACLE__PKT,
	FLYING_PKT,
	ITEM_PKT
};

typedef struct {
	uint8_t head0;
	uint8_t head1;
	player_t player0;
	player_t player1;
	obstacle_t obstacles[OBSTACLES_NUM];
	flying_t flyings[2];
	item_t items[TOTAL_ITEM_COUNT];
	bullet_t bullets[30];
	uint8_t tail;
} __attribute__((packed)) pkt_sync_init_t;

typedef struct {
	uint8_t head0;
	uint8_t head1;
	uint8_t msg;
	uint8_t tail;
} __attribute__((packed)) pkt_msg_t;

typedef struct {
	uint8_t type;
	uint8_t index;
	uint8_t active;
	float x;
	float y;
	short health;
	short score;
} __attribute__((packed)) pkt_player_t;

typedef struct {
	uint8_t type;
	uint8_t index;
	uint8_t active;
	float x;
	float y;
	short health;
} __attribute__((packed)) pkt_obstacle_t;

typedef struct {
	uint8_t type;
	uint8_t index;
	uint8_t active;
	float x;
	float y;
	short health;
} __attribute__((packed)) pkt_flying_t;

typedef struct {
	uint8_t type;
	uint8_t index;
	uint8_t active;
	uint8_t item_type;
	float x;
	float y;
} __attribute__((packed)) pkt_item_t;


void trans_input();
void trans_sync_init_1p();
void trans_sync_1p();
void trans_hello_2p();
void trans_thank_2p();

void receive_init();

void sync_init_2p();
void sync_2p();

void receive_and_update_data();

uint8_t get_sync_flag();
void set_sync_flag_zero();

uint8_t get_sync_init_flag();
void set_sync_init_flag_zero();

uint8_t get_thank_flag();
void set_thank_flag_zero();

uint8_t get_hello_flag();
void set_hello_flag_zero();


#endif /* INC_NETWORK_H_ */
