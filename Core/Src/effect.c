/*
 * effect.c
 *
 *  Created on: May 23, 2026
 *      Author: PC
 */
#include <stdint.h>
#include "effect.h"

effect_t effects[20];

void render_and_update_effects(uint32_t delta_time) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			continue;
		}

		render_texture(effects[i].x, effects[i].y, 0, 0, &effects[i].texture, delta_time);

		if (effects[i].texture.cur_frame == effects[i].texture.max_frame_count - 1 && effects[i].texture.accum_dt > effects[i].texture.next_dt) {
			effects[i].isActive = 0;
		}
	}
}

void add_shot_effect(uint8_t dir, float x, float y, float offset_x, float offset_y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x + offset_x;
			effects[i].y  = y + offset_y;

			if (dir == 1) { // left
				effects[i].texture.p_texture = image_data_left_bullet_effect_44_8_2frame;
			} else {
				effects[i].texture.p_texture = image_data_bullet_effect_44_8_2frame;
			}

			effects[i].texture.total_frame_width = 44;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 200;
			effects[i].texture.frame_width = 22;
			effects[i].texture.frame_height = 8;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 2;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}

void add_boom_effect(float x, float y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x;
			effects[i].y  = y - 102;

			effects[i].texture.p_texture = image_data_boom_explosion_550_102_11f;
			effects[i].texture.total_frame_width = 550;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 100;
			effects[i].texture.frame_width = 50;
			effects[i].texture.frame_height = 102;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 11;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}

void add_small_explosion_effect(float x, float y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x;
			effects[i].y  = y - 57 < 0 ? 0 : y - 57;

			effects[i].texture.p_texture = image_data_small_explosion_550_57_10f;
			effects[i].texture.total_frame_width = 550;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 150;
			effects[i].texture.frame_width = 55;
			effects[i].texture.frame_height = 57;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 10;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}

void add_explosion_effect(float x, float y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x;
			effects[i].y  = y - 129;

			effects[i].texture.p_texture = image_data_explosion_1200_129_10f;
			effects[i].texture.total_frame_width = 1200;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 150;
			effects[i].texture.frame_width = 120;
			effects[i].texture.frame_height = 129;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 10;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}

void add_eri_regen(float x, float y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x;
			effects[i].y  = y - 239;

			effects[i].texture.p_texture = image_data_eri_regen_210_239_6f;
			effects[i].texture.total_frame_width = 210;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 100;
			effects[i].texture.frame_width = 35;
			effects[i].texture.frame_height = 239;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 6;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}

void add_eri_dead(float x, float y) {
	for (int i = 0; i < 20; i++) {
		if (effects[i].isActive == 0) {
			effects[i].isActive = 1;
			effects[i].x  = x;
			effects[i].y  = y - 36;

			effects[i].texture.p_texture = image_data_eri_dead_440_36_10f;
			effects[i].texture.total_frame_width = 440;
			effects[i].texture.accum_dt = 0;
			effects[i].texture.next_dt = 200;
			effects[i].texture.frame_width = 44;
			effects[i].texture.frame_height = 36;
			effects[i].texture.cur_frame = 0;
			effects[i].texture.max_frame_count = 10;
			effects[i].texture.isloop = 0;
			return;
		}
	}
}
