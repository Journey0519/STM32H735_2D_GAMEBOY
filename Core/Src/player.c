/*
 * character.c
 *
 *  Created on: Apr 10, 2026
 *      Author: PC
 */

#include <stddef.h>
#include "player.h"
#include "render.h"
#include "bullet.h"
#include "sound.h"
#include "effect.h"

#define PLAYER_BOX_WIDTH (32)
#define PLAYER_BOX_HEIGHT (40)
#define PLAYER_MOVE_SPEED (0.1f)
#define CAMERA_MOVE_SPEED (0.1f)

#define JUMP_SPEED (-0.39f)
#define JUMP_ACCELERATION (0.001f)

#define BG_WIDTH (1654)
#define LCD_WIDTH (480)
#define LCD_HEIGHT (272)

extern float camera_offset[2];
extern uint8_t land_map[449888];
extern uint8_t my_camera_index;

extern const uint16_t image_data_eri_idle_top_right_192_28_6frame[5376];
extern const uint16_t image_data_eri_idle_top_letf_192_28_6frame[5376];
extern const uint16_t image_data_eri_shot_top_right_222_25_6frame[5550];
extern const uint16_t image_data_eri_shot_top_left_222_25_6frame[5550];
extern const uint16_t image_data_eri_move_bottom_right_156_26_6frame[4056];
extern const uint16_t image_data_eri_move_bottom_left_156_26_6frame[4056];

player_t players[2] = { 0, };

texture_t score_1000_text = { num_font_160_16, 160, 0, 200, 16, 16, 0, 0, 0 };
texture_t score_100_text = { num_font_160_16, 160, 0, 200, 16, 16, 0, 0, 0 };
texture_t score_10_text = { num_font_160_16, 160, 0, 200, 16, 16, 0, 0, 0 };
texture_t score_1_text = { num_font_160_16, 160, 0, 200, 16, 16, 0, 0, 0 };
texture_t px_texture[2] = { { image_data_1p_288_37_8f, 288, 0, 200, 36, 37, 0, 8, 1 }, { image_data_2p_296_37_8f, 296, 0, 200, 37, 37, 0, 8, 1 } };
texture_t px_counter[2] = { { image_data_counter_300_54_6f, 300, 0, 1000, 50, 54, 0, 6, 0 }, { image_data_counter_300_54_6f, 300, 0, 1000, 50, 54, 0, 6, 0 } };

uint8_t player_init(uint8_t index) {
	if (index == 1) {
		players[index].object = (object_t) PLAYER_INIT1_OBJECT;
	} else {
		players[index].object = (object_t) PLAYER_INIT0_OBJECT;
	}
	players[index].active = 1;
	players[index].state = (state_t) PLAYER_INIT_STATE;
	players[index].top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_RIGHT;
	players[index].bottom = (texture_t) PLAYER_IDLE_TEXTURE_BOTTOM_RIGHT;
	//{ image_data_eri_jump_bottom_right_144_30_6frame, 144, 0, 24, 30, 0, 6, 1 };
	players[index].damage = PLAYER_INIT_DAMAGE;
	players[index].health.health = (1000);
	players[index].health.health_bar = (texture_t) PLAYER_HEALTH_BAR;
	players[index].health.health_bar_ani = (texture_t) PLAYER_HEALTH_BAR_ANI;
	players[index].health.health_bar_empty = (texture_t) PLAYER_HEALTH_BAR_EMPTY;
	players[index].v_y = 0;

	return index;
}

void player_update(uint8_t index, input_t input, uint32_t delta_time) {
	if (players[index].active == 0) {
		players[index].regen_time -= delta_time;

   		if (index == my_camera_index) {
			render_texture(camera_offset[my_camera_index] + 225, 15, 0, 0, &px_counter[index], delta_time);
		}

		if (players[index].regen_time < 0 && *(uint8_t*)&input != 0) {
			object_t regen_pos = players[index].object;
			player_init(index);
			players[index].object = regen_pos;
			add_eri_regen(players[index].object.position_x, players[index].object.position_y);
			add_start_sound();
		}
		return;
	}

	if (players[index].health.health <= 0) {
		players[index].active = 0;
		players[index].regen_time = 5000;
		px_counter[index].cur_frame = 0;
		add_eri_dead(players[index].object.position_x, players[index].object.position_y);
		add_eri_dead_sound();
	}


	object_t before_object = players[index].object;
	object_t next_object = players[index].object;

	float next_x = players[index].object.position_x;
	float next_y = players[index].object.position_y;
	int next_x_index;
	int next_y_index;

	update_state(index, input);

	next_x += (input.right * delta_time * PLAYER_MOVE_SPEED) - (input.left * delta_time * PLAYER_MOVE_SPEED);
	next_y += (input.down * delta_time * PLAYER_MOVE_SPEED) - (input.up * delta_time * PLAYER_MOVE_SPEED);

	//update position x
	next_x = next_x < 0 ? 0 : next_x;
	next_x = next_x > 1600.0f ? 1600.0f : next_x;

	//check position  y collision
	next_object.position_x = next_x;

	if (check_player_collision(next_object, index) == 1) {
		next_x = players[index].object.position_x; //기존꺼로
		next_object.position_x = next_x;
	}

	next_x_index = (uint32_t) next_x;
	next_y_index = (uint32_t) next_y;
	next_y_index = next_y_index > LCD_HEIGHT ? 0 : next_y_index;

	players[index].object.position_x = next_x;

	//camera offset
	if (camera_offset[index] > (BG_WIDTH - LCD_WIDTH)) {
		camera_offset[index] = BG_WIDTH - LCD_WIDTH;
	} else if (camera_offset[index] < 0) {
		camera_offset[index] = 0;
	} else if (next_x - camera_offset[index] > 320.0f) {
		camera_offset[index] += CAMERA_MOVE_SPEED * delta_time;
	} else if (next_x - camera_offset[index] < 160.0f - players[index].object.width) {
		camera_offset[index] -= CAMERA_MOVE_SPEED * delta_time;
	}


//	next_x_index = (uint32_t) next_x;
//	next_y_index = (uint32_t) next_y;

	if (land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0x00) {
		next_y = players[index].object.position_y;
	}

	//active gravity
	if (players[index].v_y >= 0.0f && land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0xFF) { //떨어지는데 지면인데?
			players[index].v_y = 0;
	} else { //지면아닌데 상승중? 지면인데 상승중? 떨어지는중인데 지면 아닌데?
		players[index].v_y += JUMP_ACCELERATION * delta_time; //떨어지기 가속
	}

	//check position  y collision
	next_y = (next_y + (players[index].v_y * delta_time)) > 272 ? 272 : (next_y + (players[index].v_y * delta_time));
	next_object.position_y = next_y;
	if (check_player_collision(next_object, index) == 1) {
		next_y = players[index].object.position_y; //기존꺼로
		next_object.position_y = next_y;
		if (players[index].v_y > 0.0f) {
			players[index].v_y = 0.0f;
		} else {
			players[index].v_y = 0.01f;
		}

	}


	//update x, y position
	players[index].object.position_y = next_object.position_x;
	players[index].object.position_y = next_object.position_y;

	update_player_collision_map(before_object, next_object, index);
}

void player_render(uint8_t index, uint32_t delta_time) {
	if (players[index].active == 0) {
		return;
	}

	uint32_t* p_texture_with_frame = NULL;
	float start_x = players[index].object.position_x;
	float start_y = players[index].object.position_y - players[index].object.hegiht; //히트박스의 왼쪽 윗점을 기준으로 render
	float bottom_offset_x = 0;
	float bottom_offset_y = 0;

	if (players[index].state.look_left) {
		if (players[index].state.look_up) {

		} else if (players[index].state.look_down) {

		} else if (players[index].state.shot) { //정면 샷
			bottom_offset_x = ERI_SHOT_BOTTOM_LEFT_X_OFFSET;
			bottom_offset_y = ERI_SHOT_BOTTOM_Y_OFFSET;
 		} else { //상체 가만히 idle이면서 멈춰있거나 MOVE
			bottom_offset_x = ERI_IDLE_MOVE_BOTTOM_LEFT_X_OFFSET;
			bottom_offset_y = ERI_IDLE_MOVE_BOTTOM_Y_OFFSET;
		}
	} else if (players[index].state.look_right) {
		if (players[index].state.look_up) {

		} else if (players[index].state.look_down) {

		} else if (players[index].state.shot) {
			bottom_offset_x = ERI_SHOT_BOTTOM_RIGHT_X_OFFSET;
			bottom_offset_y = ERI_SHOT_BOTTOM_Y_OFFSET;
		} else {
			bottom_offset_x = ERI_IDLE_MOVE_BOTTOM_RIGHT_X_OFFSET;
			bottom_offset_y = ERI_IDLE_MOVE_BOTTOM_Y_OFFSET;
		}
	}

	render_texture(start_x, start_y, bottom_offset_x, bottom_offset_y, &players[index].bottom, delta_time);
	render_texture(start_x, start_y, 0, 0, &players[index].top, delta_time); //상체 그리기

	if (players[index].state.look_left) { //조준선
		render_box_R2M(start_x, start_y, -1 - (80), 2, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, -1 - (80), 8, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, -1 - (80), 14, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, -1 - (86), 8, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, -1 - (74), 8, 3, 3, 0xFFFFFFFF, delta_time);
	} else {
		render_box_R2M(start_x, start_y, 32 + (80), 2, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, 32 + (80), 8, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, 32 + (80), 14, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, 32 + (86), 8, 3, 3, 0xFFFFFFFF, delta_time);
		render_box_R2M(start_x, start_y, 32 + (74), 8, 3, 3, 0xFFFFFFFF, delta_time);
	}

	health_bar_render(index, delta_time);

	uint16_t score = players[index].score;

	uint16_t score_1000 = score/1000;
	score_1000_text.cur_frame = score_1000;
	score_1000_text.max_frame_count = score_1000 + 1;
	score -= score_1000*1000;

	uint16_t score_100 = score / 100;
	score_100_text.cur_frame = score_100;
	score_100_text.max_frame_count = score_100 + 1;
	score -= score_100*100;

	uint16_t score_10 = score / 10;
	score_10_text.cur_frame = score_10;
	score_10_text.max_frame_count = score_10 + 1;
	score -= score_10*10;

	uint16_t score_1 = score;
	score_1_text.cur_frame = score_1;
	score_1_text.max_frame_count = score_1 + 1;

	float score_x;
	if (index == 0) {
		score_x = 10.f;
	} else {
		score_x = (float)(470 - 16 * 4);
	}

	render_texture(score_x + camera_offset[my_camera_index], 10, 0, 0, &score_1000_text, delta_time);
	render_texture(score_x + 16 + camera_offset[my_camera_index], 10, 0, 0, &score_100_text, delta_time);
	render_texture(score_x + 16 * 2 + camera_offset[my_camera_index], 10, 0, 0, &score_10_text, delta_time);
	render_texture(score_x + 16 * 3 + camera_offset[my_camera_index], 10, 0, 0, &score_1_text, delta_time);

	render_texture(players[index].object.position_x, players[index].object.position_y - 40 - 32, 0, 0, &px_texture[index], delta_time);
}

static void update_state(uint8_t index, input_t input) {
	static input_t before_input = { 0, };
	texture_t top = players[index].top;
	texture_t bottom = players[index].bottom;
	state_t state = players[index].state;
	uint8_t input_byte  = *(uint8_t*)&input;


	if (state.shot == 1 &&  top.cur_frame == top.max_frame_count - 1 && top.accum_dt >= top.next_dt) { //발싸 끝
		state.shot = 0;
		if (state.look_left) {
			top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_LEFT;
		} else {
			top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_RIGHT;
		}
	}

	if (state.jump == 1 && players[index].v_y  == 0) { //점프 끝 && bottom.cur_frame == bottom.max_frame_count -1 && bottom.accum_dt >= bottom.next_dt
		state.jump = 0;                                //v.y 부동소수점이라 가속도 계속 더해줘도 최고점에서 딱 0안됌. 지면 밟아야지만 0됌
		state.move = 0;

		if (state.look_left) {
			bottom = (texture_t) PLAYER_IDLE_TEXTURE_BOTTOM_LEFT;
		} else {
			bottom = (texture_t) PLAYER_IDLE_TEXTURE_BOTTOM_RIGHT;
		}
	}


	if (state.shot == 0 && state.jump == 0 && state.idle == 0 && (uint8_t)(input_byte << 4) == 0x00) {
		state.look_up = 0;
		state.look_down = 0;
		state.move = 0;

		state.idle = 1; //idle상태로

		if (state.look_left) {
			top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_LEFT;
			bottom = (texture_t) PLAYER_IDLE_TEXTURE_BOTTOM_LEFT;
		} else {
			top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_RIGHT;
			bottom = (texture_t) PLAYER_IDLE_TEXTURE_BOTTOM_RIGHT;
		}
	}

	if (input.left) {
		if (state.look_right) {
			state.look_right = 0;
			switch ((uint32_t) top.p_texture) {
			case ERI_IDLE_TOP_RIGHT:
					top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_LEFT;
				break;
			case ERI_SHOT_DOWN_RIGHT:
					top = (texture_t) PLAYER_SHOT_DOWN_TEXTURE_TOP_LEFT;
				break;
			case ERI_SHOT_TOP_RIGHT:
					top = (texture_t) PLAYER_SHOT_TEXTURE_TOP_LEFT;
				break;
			case ERI_SHOT_UP_RIGHT:
					top = (texture_t) PLAYER_SHOT_UP_TEXTURE_TOP_LEFT;
				break;
			default:
			}
			top.accum_dt = players[index].top.accum_dt;
			top.cur_frame = players[index].top.cur_frame;
		};

		state.look_left = 1;


		if (state.move == 0 || bottom.p_texture == ERI_MOVE_RIGHT) {
			bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_LEFT;
			state.move = 1;
			state.idle = 0;
		}

	} else if (input.right) {
		if (state.look_left) {
			state.look_left = 0;
			switch ((uint32_t) top.p_texture) {
			case ERI_IDLE_TOP_LEFT:
					top = (texture_t) PLAYER_IDLE_TEXTURE_TOP_RIGHT;
				break;
			case ERI_SHOT_DOWN_LEFT:
					top = (texture_t) PLAYER_SHOT_DOWN_TEXTURE_TOP_RIGHT;
				break;
			case ERI_SHOT_TOP_LEFT:
					top = (texture_t) PLAYER_SHOT_TEXTURE_TOP_RIGHT;
				break;
			case ERI_SHOT_UP_LEFT:
					top = (texture_t) PLAYER_SHOT_UP_TEXTURE_TOP_RIGHT;
				break;
			default:
			}
			top.accum_dt = players[index].top.accum_dt;
			top.cur_frame = players[index].top.cur_frame;
		};
		state.look_right = 1;

		if (state.move == 0 || bottom.p_texture == ERI_MOVE_LEFT) {
			bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_RIGHT;
			state.move = 1;
			state.idle = 0;
		}
	}

	if (input.up || input.down) { //todo: 여기 check 위/아래 더 갈 수 있는지 -> 못가면 상태 look_up/down
		if (state.move == 0) {
			if (state.look_left) {
					bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_LEFT;
			} else {
					bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_RIGHT;
			}
			state.idle = 0;
			state.move = 1;
		} else {
			if (state.look_left) {
				if (bottom.p_texture == ERI_MOVE_RIGHT) {
					bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_LEFT;
				}
			} else if (bottom.p_texture == ERI_MOVE_LEFT) {
				bottom = (texture_t) PLAYER_MOVE_TEXTURE_BOTTOM_RIGHT;
			}
		}

	} else {
		state.look_up = 0;
		state.look_down = 0;
	}

	if (state.shot == 0 && input.shot) {          //발싸!!
		add_shot_sound();


	//	add_shot_effect(players[index].object.position_x, players[index].object.position_y - players[index].object.hegiht , -21, 6); 왼
		state.idle = 0;
		state.shot = 1;

		uint8_t bullet_state = (*(uint8_t*)&state) >> 3;
		add_bullet(bullet_state, players[index].object);

		switch (((*(uint16_t*)(&state)) >> 4) & 0x0003) {
		case 0x0000:
			if (state.look_left) {
				top = (texture_t) PLAYER_SHOT_TEXTURE_TOP_LEFT;
				add_shot_effect(state.look_left, players[index].object.position_x, players[index].object.position_y - players[index].object.hegiht , -22, 6);
			} else {
				top = (texture_t) PLAYER_SHOT_TEXTURE_TOP_RIGHT;
				add_shot_effect(state.look_left, players[index].object.position_x, players[index].object.position_y - players[index].object.hegiht , 37, 6);
			}
			break;
		case 0x0001: //UP
			if (state.look_left) {
				top = (texture_t) PLAYER_SHOT_UP_TEXTURE_TOP_LEFT;
			} else {
				top = (texture_t) PLAYER_SHOT_UP_TEXTURE_TOP_RIGHT;
			}
			break;
		case 0x0002: //DOWN
			if (state.look_left) {
				top = (texture_t) PLAYER_SHOT_DOWN_TEXTURE_TOP_LEFT;
			} else {
				top = (texture_t) PLAYER_SHOT_DOWN_TEXTURE_TOP_RIGHT;
			}
			break;
		default:
		}
	}

	if (state.jump == 0 && input.jump) {         // 쩜프
		add_jump_sound();

		state.idle = 0;
		state.jump = 1;
		state.move = 1;
		players[index].v_y = JUMP_SPEED;
		if (state.look_left) {
			bottom = (texture_t) PLAYER_JUMP_TEXTURE_BOTTOM_LEFT;
		} else {
			bottom = (texture_t) PLAYER_JUMP_TEXTURE_BOTTOM_RIGHT;
		}
	}

	players[index].state = state;
	players[index].top = top;
	players[index].bottom = bottom;

	before_input = input;

	return;
}


static void health_bar_render(uint8_t index, uint32_t delta_time) {
	float start_x = players[index].object.position_x;
	float start_y = players[index].object.position_y - players[index].object.hegiht + PLAYER_HEALTH_BAR_OFFSET_Y; //히트박스의 왼쪽 윗점을 기준으로 render

	render_texture_no_blend(start_x, start_y, 0, 0, &players[index].health.health_bar_empty, delta_time);
	uint8_t player_health_width = 32 * players[index].health.health / 1000.0f;
	players[index].health.health_bar.frame_width = player_health_width;

	if (players[index].health.health_bar_ani.frame_width > player_health_width) {
		if (players[index].health.health_bar_ani.accum_dt >= players[index].health.health_bar_ani.next_dt)
		players[index].health.health_bar_ani.frame_width--;;
	} else {
		players[index].health.health_bar_ani.frame_width = player_health_width;
	}

	render_texture_no_blend(start_x, start_y, 0, 0, &players[index].health.health_bar_ani, delta_time);
	render_texture_no_blend(start_x, start_y, 0, 0, &players[index].health.health_bar, delta_time);
}
