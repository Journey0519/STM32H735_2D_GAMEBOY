/*
 * car.h
 *
 *  Created on: Jun 13, 2026
 *      Author: PC
 */

#ifndef INC_OBSTACLE_H_
#define INC_OBSTACLE_H_

#include "object.h"
#include "texture.h"


#define OBSTACLES_NUM (10)

typedef struct {
	uint8_t active;
	object_t object;
	int16_t health;
	texture_t texture;
	uint8_t hit;
	short regen_time;
	float v_y;
} obstacle_t;

void init_obstacles();
void render_and_update_obstacles(uint8_t delta_time);

#endif /* INC_OBSTACLE_H_ */
