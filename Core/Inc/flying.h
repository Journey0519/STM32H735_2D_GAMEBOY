/*
 * flying.h
 *
 *  Created on: Jun 14, 2026
 *      Author: PC
 */

#ifndef INC_FLYING_H_
#define INC_FLYING_H_

#include "object.h"
#include "texture.h"

typedef struct {
	uint8_t active;
	object_t obj;
	texture_t flying;
	float phase;
	short health;
	short regen_time;
	short reload;
} flying_t;

void init_flying();
void render_and_update_flyings(uint32_t delta_time);

#endif /* INC_FLYING_H_ */
