/*
 * item.h
 *
 *  Created on: Jun 14, 2026
 *      Author: PC
 */

#ifndef INC_ITEM_H_
#define INC_ITEM_H_

#include <stdint.h>
#include "object.h"
#include "texture.h"

#define TOTAL_ITEM_COUNT (15)

typedef enum {
	heal = 0,
	point = 1,
	boom = 2
} item_type_t;

typedef struct {
	uint8_t active;
	uint8_t type;
	object_t object;
	texture_t item_text;
	float v_y;
} item_t;

void init_items();
void render_and_update_items(uint32_t delta_time);
void add_item(item_type_t item_type, float x, float y);

#endif /* INC_ITEM_H_ */
