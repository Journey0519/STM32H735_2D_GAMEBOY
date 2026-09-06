/*
 * flying.c
 *
 *  Created on: Jun 14, 2026
 *      Author: PC
 */

#include "flying.h"
#include "render.h"
#include "math.h"
#include "rand.h"
#include "item.h"
#include "player.h"
#include "effect.h"
#include "sound.h"

#define CENTER_Y 170
#define WAVE_PERIOD 800
#define WAVE_AMPLITUDE 80
#define PI 3.14159265f
flying_t flyings[2];

extern player_t players[2];
extern float camera_offset[2];

void init_flying() {
	flyings[0].active = 1;
	flyings[0].obj.position_x = 0;
	flyings[0].obj.position_y = 80;
	flyings[0].obj.width = 80;
	flyings[0].obj.hegiht = 41;
	flyings[0].flying = (texture_t){ image_data_flying_1040_41_13f, 1040, 0, 100, 80, 41, 0, 13, 1 };
	flyings[0].phase = 0;
	flyings[0].health = 1000;
	flyings[0].regen_time = 0;
	flyings[1].reload = 3000;


}

void render_and_update_flyings(uint32_t delta_time) {
	for (int i = 0; i < 2; i ++) {
		if (flyings[i].active == 1) {
			if (flyings[i].health == 0) {
				flyings[i].active = 0;
				flyings[i].regen_time = 10000;
				add_small_explosion_effect(flyings[i].obj.position_x + 20.f, flyings[i].obj.position_y);

				if (flyings[i].obj.position_x > camera_offset[my_camera_index] && flyings[i].obj.position_x < camera_offset[my_camera_index] + 480) {
					add_explosion_sound();
				}
			}

			render_texture(flyings[i].obj.position_x, flyings[i].obj.position_y, i, -flyings[i].obj.hegiht, &flyings[i].flying, delta_time);

			flyings[i].phase += 0.001 * delta_time;

			if (i == 0) {
				flyings[i].obj.position_x += 0.15 * delta_time;
				if (flyings[i].obj.position_x > 1654) {
					flyings[i].obj.position_x = -81;
				}

			} else {
				flyings[i].obj.position_x -= 0.15 * delta_time;
				if (flyings[i].obj.position_x < -80) {
								flyings[i].obj.position_x = 1660;
				}
			}

			flyings[i].obj.position_y = 80 + (int16_t) (20 * sinf(flyings[i].phase));


			flyings[i].reload -= delta_time;

			uint16_t distance_x_1p = abs((int)(players[0].object.position_x - flyings[i].obj.position_x));
			uint16_t distance_x_2p = abs((int)(players[1].object.position_x - flyings[i].obj.position_x));
			uint8_t random = FastRand() % 2;
			uint16_t distance = distance_x_1p <  distance_x_2p ? distance_x_1p : distance_x_2p;
			uint16_t attack_range = 300 - (i) * 200 - (random * 50);

			if (flyings[i].reload < 0 && flyings[i].obj.position_x > 50 && flyings[i].obj.position_x < 1600 && (distance < attack_range)) {
				if (flyings[i].obj.position_x > camera_offset[my_camera_index] && flyings[i].obj.position_x < camera_offset[my_camera_index] + 480) {
					add_drop_sound();
				}


				if (i == 0) {
					add_item(*(item_type_t*)&random, flyings[i].obj.position_x + 33, flyings[i].obj.position_y);
					flyings[i].reload = 5000;
				} else {
					add_item(boom, flyings[i].obj.position_x + 33, flyings[i].obj.position_y);
					flyings[i].reload = 1000;
				}

			}

		} else {
			flyings[i].regen_time -= delta_time;

			if (flyings[i].regen_time < 0) {
				switch (i) {
				case 0:
					flyings[i].active = 1;
					flyings[i].obj.position_x = 0;
					flyings[i].obj.position_y = 80;
					flyings[i].obj.width = 80;
					flyings[i].obj.hegiht = 41;
					flyings[i].flying = (texture_t){ image_data_flying_1040_41_13f, 1040, 0, 100, 80, 41, 0, 13, 1 };
					flyings[i].phase = 0;
					flyings[i].health = 1000;
					flyings[i].regen_time = 0;

					break;

				case 1:
					flyings[1].active = 1;
					flyings[1].obj.position_x = 1654;
					flyings[1].obj.position_y = 80;
					flyings[1].obj.width = 79;
					flyings[1].obj.hegiht = 38;
					flyings[1].flying = (texture_t){ image_data_flying2_316_38_4f, 316, 0, 100, 79, 38, 0, 4, 1 };
					flyings[1].phase = 1.57;
					flyings[1].health = 1000;
					flyings[1].regen_time = 0;
					flyings[1].reload = 3000;

					break;
				default:

				}

			}
		}
	}
}


