/*
 * object.c
 *
 *  Created on: May 3, 2026
 *      Author: PC
 */

#include "object.h"

#include "math.h"
#include "collision.h"
#include "player.h"

#define CENTER_Y 170
#define WAVE_PERIOD 800   // 한 화면에 2개 정도 보이도록 설정
#define WAVE_AMPLITUDE 80 // 높낮이
#define PI 3.14159265f


extern collision_t collision_map[9][52];
extern player_t players[2];

static const uint8_t NULL_INDEX = -1;

//void init_test_object() {
//	for (int i = 0 ; i < 15; i++) {
//		objects[i].position_x = 50 + 100 * i;
//		objects[i].position_y = 170;
//		objects[i].width = 32;
//		objects[i].hegiht = 32;
//		objects[i].test_health = 1000;
//		objects[i].test_isActive = 1;
//
//		add_object_collision_map(objects[i], i);
//	}
//}

//void update_test_object(uint32_t delta_time) {
//	for (int i = 0 ; i < 15; i++) {
//
//
//		if (objects[i].test_isActive == 0) {
//			continue;
//		}
//
//		if (objects[i].test_health == 0) {
//			objects[i].test_isActive = 0;
//			remove_object_collision_map(i);
//			continue;
//		}
//
//		object_t before_obj = objects[i];
//		float next_y;
//
//		object_t next_obj = objects[i];
//		object_phase[i] += 0.001 * delta_time;
//		float phase = ((objects[i].position_x / WAVE_PERIOD) * 2.0f * 3.14159f);
//		next_y = objects[i].position_y;
//		next_obj.position_y = objects[i].position_y;
//
//		int x_start_index = (int)(next_obj.position_x) >> DIV_COLLISION_IDX;
//		int x_end_index = (int)(next_obj.position_x + next_obj.width) >> DIV_COLLISION_IDX;
//		int y_start_index = (int)(next_obj.position_y - next_obj.hegiht) >> DIV_COLLISION_IDX;
//		int y_end_index = (int)(next_obj.position_y) >> DIV_COLLISION_IDX;
//		int is_hit = 0;
//		uint8_t other_index = NULL_INDEX;
//
//		for (int j = x_start_index; j <= x_end_index; j++) {
//			for (int k = 0; k < 2; k++) {
//				other_index = collision_map[y_start_index][j].player_index_arr[k];
//				if (other_index != NULL_INDEX && other_index != i) {
//					is_hit = check_hitbox_collision(next_obj, players[other_index].object);
//
//					if (is_hit == 1) {
//						goto hit;
//					}
//				}
//
//				if (y_start_index == y_end_index) {
//					continue;
//				}
//
//				other_index = collision_map[y_end_index][j].player_index_arr[k];
//				if (other_index != NULL_INDEX  && other_index != i) {
//					is_hit = check_hitbox_collision(next_obj, players[other_index].object);
//
//					if (is_hit == 1) {
//						goto hit;
//					}
//				}
//			}
//		}
//
//		if (y_start_index + 1 > y_end_index - 1) {
//			goto next;
//		}
//
//		for (int j = y_start_index + 1; j <= y_end_index - 1; j++) {
//			for (int k = 0; k < 2; k++) {
//				other_index = collision_map[j][x_start_index].player_index_arr[k];
//				if (other_index != NULL_INDEX  && other_index != i) {
//					is_hit = check_hitbox_collision(next_obj, players[other_index].object);
//
//					if (is_hit == 1) {
//						goto hit;
//					}
//				}
//
//				if (x_start_index == x_end_index) {
//					continue;
//				}
//
//				other_index = collision_map[j][x_end_index].player_index_arr[k];
//				if (other_index != NULL_INDEX  && other_index != i) {
//					is_hit = check_hitbox_collision(next_obj, players[other_index].object);
//
//					if (is_hit == 1) {
//						goto hit;
//					}
//				}
//			}
//		}
//
//		goto next;
//
//hit:
//		object_phase[i] -= 0.001 * delta_time;
//		next_y = before_obj.position_y;
//
//next:
//		objects[i].position_y = next_y;
//		update_objcect_collision_map(before_obj, objects[i], i);
//
//		render_hitbox(objects[i]);
//	}
//}

//todo: 물체 사라지면 충돌맵 remove하기
//x 충돌인지 y충돌인지해서 y충돌일시 x는 움직이게금하기
