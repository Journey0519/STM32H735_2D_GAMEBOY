/*
 * bullet.h
 *
 *  Created on: May 3, 2026
 *      Author: PC
 */

#ifndef INC_BULLET_H_
#define INC_BULLET_H_

#include <stdint.h>
#include "object.h"

enum {
	BULLET_RIGHT = 0,
	BULLET_LEFT = 1,
	BULLET_UP = 2,
	directionDOWN = 3
};

typedef struct {
	uint8_t is_active : 1;
	uint8_t is_up : 1;
	uint8_t is_down : 1;
	uint8_t is_left : 1;
	uint8_t is_right : 1;
	uint8_t padding : 3;
	object_t obj;
} bullet_t;


void init_bullets();
void render_and_update_bullets(uint32_t delta_time);
void add_bullet(uint8_t bullet_state, object_t player_obj);
void render_bullets(uint32_t delta_time);
#endif /* INC_BULLET_H_ */
