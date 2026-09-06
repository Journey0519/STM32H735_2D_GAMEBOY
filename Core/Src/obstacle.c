/*
 * ocstacle.c
 *
 *  Created on: Jun 13, 2026
 *      Author: PC
 */

#include <resource/texture/car_texture.h>
#include <stdlib.h>
#include <stdint.h>
#include "obstacle.h"
#include "collision.h"
#include "rand.h"
#include "render.h"
#include "effect.h"
#include "sound.h"
#include "player.h"

#define ACCELERATION (0.0001f)
#define LCD_HEIGHT (272)

extern uint8_t land_map[449888];
extern player_t players[2];
extern float camera_offset[2];

obstacle_t obstacles[OBSTACLES_NUM];
const texture_t cars[5] = { CAR1_INIT, CAR2_INIT, CAR3_INIT, CAR4_INIT, CAR5_INIT };
const uint8_t car_width[5] =  { 87, 73, 73, 94, 87 };
const uint8_t car_height[5] = { 51, 43, 53, 57, 46 };

void init_obstacles() {
	for (int i  = 0; i < 10; i++) {
		uint8_t car_index = i % 5;
		obstacles[i].active = 1;
		obstacles[i].object.width = car_width[car_index];
		obstacles[i].object.hegiht = car_height[car_index];
		obstacles[i].object.position_x =  150 * i + (i % 2) * (-20);
		obstacles[i].object.position_y = 270 + (i % 2) * (- 20);
		obstacles[i].health = 600;
		obstacles[i].v_y = ACCELERATION;
		obstacles[i].hit = 0;
		obstacles[i].texture = cars[car_index];
		obstacles[i].regen_time = 5000;

		add_object_collision_map(obstacles[i].object, i, OBSTACLE);
	}

}

void render_and_update_obstacles(uint8_t delta_time) {
	for (int i = 0; i < OBSTACLES_NUM; i++) {
		if (obstacles[i].active == 0) {
			obstacles[i].regen_time -= delta_time;

			if (obstacles[i].regen_time < 0) {
				uint8_t car_index = i % 5;
				obstacles[i].active = 1;
				obstacles[i].object.width = car_width[car_index];
				obstacles[i].object.hegiht = car_height[car_index];
				obstacles[i].object.position_x =  150 * i + (i % 2) * (-20);
				obstacles[i].object.position_y = 0;//270 + (i % 2) * (- 20);
				obstacles[i].health = 1000;
				obstacles[i].v_y = ACCELERATION;
				obstacles[i].hit = 0;
				obstacles[i].texture = cars[car_index];
				obstacles[i].regen_time = 5000;

				add_object_collision_map(obstacles[i].object, i, OBSTACLE);
			}
			continue;
		}

		if (obstacles[i].health == 0) {
			remove_object_collision_map(obstacles[i].object, i, OBSTACLE);
			obstacles[i].active = 0;
			add_explosion_effect(obstacles[i].object.position_x, obstacles[i].object.position_y);

			if (obstacles[i].object.position_x > camera_offset[my_camera_index] && obstacles[i].object.position_x < camera_offset[my_camera_index] + 480) {
				add_explosion_sound();
			}

			continue;
		}

		if (obstacles[i].hit == 1) {
			obstacles[i].hit = 0;
			obstacles[i].texture.cur_frame = 0;
		}

		render_texture(obstacles[i].object.position_x, obstacles[i].object.position_y - obstacles[i].object.hegiht, 0, 0, &obstacles[i].texture, delta_time);

		object_t before_object = obstacles[i].object;
		object_t next_object = obstacles[i].object;

		float next_x = obstacles[i].object.position_x;
		float next_y = obstacles[i].object.position_y;
		int next_x_index = 0;
		int next_y_index = 0;

		next_x_index = (uint32_t) next_x;
		next_y_index = (uint32_t) next_y;
		next_y_index = next_y_index > LCD_HEIGHT ? 0 : next_y_index;


		if (land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0x00) {
			next_y = obstacles[i].object.position_y;
		}

		//active gravity
		if (obstacles[i].v_y >= 0.0f && land_map[(int)next_x_index + (int)(next_y_index) * 1654] == 0xFF && obstacles[i].object.position_y > 270 + (i % 2) * (- 20)) { //떨어지는데 지면인데?
				obstacles[i].v_y = 0;
		} else { //지면아닌데 상승중? 지면인데 상승중? 떨어지는중인데 지면 아닌데?
			obstacles[i].v_y += ACCELERATION * delta_time; //떨어지기 가속
		}

		//check position  y collision
		next_y = (next_y + (obstacles[i].v_y * delta_time)) > 272 ? 272 : (next_y + (obstacles[i].v_y * delta_time));
		next_object.position_y = next_y;
		if (check_object_collision(next_object, i)) {
			next_object.position_y = before_object.position_y; //기존꺼로
			obstacles[i].v_y = 0.0f;
		}


		//update x, y position
		obstacles[i].object.position_y = next_object.position_x;
		obstacles[i].object.position_y = next_object.position_y;

		update_objcect_collision_map(before_object, next_object, i, OBSTACLE);
	}
}
