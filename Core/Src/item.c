/*
 * item.c
 *
 *  Created on: Jun 14, 2026
 *      Author: PC
 */

#include "item.h"
#include "render.h"
#include "collision.h"
#include "player.h"
#include "effect.h"
#include "object.h"
#include "assert.h"
#include "sound.h"

#define ACCELERATION (0.00001f)
#define LCD_HEIGHT (272)
#define COIN_INIT { image_data_coin_135_16_9_frame, 135, 0, 100, 15, 16, 0, 9, 1}
#define FISH_INIT { image_data_fish_190_20_6_frame, 190, 0, 100, 31, 20, 0, 6, 1 }
#define BOOM_INIT { image_data_boom_245_36_7f, 245, 0, 100, 35, 36, 0, 7, 1 }

#define POINT (13);
#define HEAL (50);

extern uint8_t land_map[449888];
extern player_t players[2];
extern float camera_offset[2];
extern uint8_t my_camera_index;

item_t items[TOTAL_ITEM_COUNT] __attribute__((section(".hyperram_data"), aligned(32)));

const texture_t item_textures[3] = { COIN_INIT, FISH_INIT, BOOM_INIT };
const uint8_t item_widths[3] =  { 15, 31, 35  };
const uint8_t item_heights[3] = { 16, 20, 36 };

void init_items() {
	memset(items, 0, sizeof(item_t) * TOTAL_ITEM_COUNT);
}

void render_and_update_items(uint32_t delta_time) {
	for (int i = 0; i < TOTAL_ITEM_COUNT; i++) {
		if (items[i].active == 0) {
			continue;
		}

		render_texture(items[i].object.position_x, items[i].object.position_y - items[i].object.hegiht, 0, 0, &items[i].item_text, delta_time);

		object_t before_object = items[i].object;
		object_t next_object = items[i].object;

		float next_x = items[i].object.position_x;
		float next_y = items[i].object.position_y;
		int next_x_index = 0;
		int next_y_index = 0;

		next_x_index = (uint32_t) next_x;
		next_y_index = (uint32_t) next_y;
		next_y_index = next_y_index > LCD_HEIGHT ? 0 : next_y_index;


		if (land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0x00) {
			next_y = items[i].object.position_y;
		}

		//active gravity
		if (items[i].v_y >= 0.0f && land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0xFF) { //떨어지는데 지면인데?
			items[i].v_y = 0;
		} else { //지면아닌데 상승중? 지면인데 상승중? 떨어지는중인데 지면 아닌데?
			items[i].v_y += ACCELERATION * delta_time; //떨어지기 가속
		}

		//check position  y collision
		next_y = (next_y + (items[i].v_y * delta_time)) > 272 ? 272 : (next_y + (items[i].v_y * delta_time));
		next_object.position_y = next_y;
		if (check_object_collision(next_object, -1)) {
			next_object.position_y = before_object.position_y; //기존꺼로
			items[i].v_y = 0.0f;
		}

		//update x, y position
		items[i].object.position_y = next_object.position_x;
		items[i].object.position_y = next_object.position_y;

		switch (items[i].type) {
		case 0:
			if (check_hitbox_collision(items[i].object, players[0].object)) {
				players[0].score += POINT;
				items[i].active = 0;

				if (my_camera_index == 0) {
					add_coin_sound();
				}
			} else if (check_hitbox_collision(items[i].object, players[1].object)) {
				players[1].score += POINT;
				items[i].active = 0;

				if (my_camera_index == 1) {
					add_coin_sound();
				}
			}
			break;
		case 1:
			if (check_hitbox_collision(items[i].object, players[0].object)) {
				players[0].health.health += HEAL;
				if (players[0].health.health > 1000) {
					players[0].health.health = 1000;
				}
				items[i].active = 0;

				if (my_camera_index == 0) {
					add_fish_sound();
				}
			} else if (check_hitbox_collision(items[i].object, players[1].object)) {
				players[1].health.health += HEAL;
				if (players[1].health.health > 1000) {
					players[1].health.health = 1000;
				}
				items[i].active = 0;

				if (my_camera_index == 1) {
					add_fish_sound();
				}
			}

			break;
		case 2:
			if (items[i].v_y == 0) {
				items[i].active = 0;
				add_boom_effect(items[i].object.position_x, items[i].object.position_y);

				if (items[i].object.position_x > camera_offset[my_camera_index] && items[i].object.position_x < camera_offset[my_camera_index] + 480) {
					add_boom_sound();
				}

				object_t boom_range = { items[i].object.position_x - 5, items[i].object.position_y + 5, items[i].object.width + 5, items[i].object.hegiht + 5 };

				if (check_hitbox_collision(boom_range, players[0].object)) {
					players[0].health.health -= 300;
				} else if (check_hitbox_collision(boom_range, players[1].object)) {
					players[1].health.health -= 300;
				}

			}

			break;
		default:
			assert(0);
		}

	}
}


void add_item(item_type_t item_type, float x, float y) {
	for (int i = 0; i < TOTAL_ITEM_COUNT; i++)  {
		if (items[i].active == 1) {
			continue;
		}

		uint8_t item_type_index = *(uint8_t*)&item_type;
		items[i].active = 1;
		items[i].object.position_x = x;
		items[i].object.position_y = y;
		items[i].object.width = item_widths[item_type_index];
		items[i].object.hegiht = item_heights[item_type_index];
		items[i].type = item_type_index;
		items[i].item_text = item_textures[item_type_index];
		items[i].v_y = 0.1;

		return;
	}
}
