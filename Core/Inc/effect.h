/*
 * effect.h
 *
 *  Created on: May 23, 2026
 *      Author: PC
 */

#ifndef INC_EFFECT_H_
#define INC_EFFECT_H_

#include "texture.h"
#include "render.h"

typedef struct {
	uint8_t isActive;
	float x;
	float y;
	texture_t texture;
} effect_t;

void render_and_update_effects(uint32_t delta_time);
void add_shot_effect(uint8_t dir, float x, float y, float offset_x, float offset_y);
void add_boom_effect(float x, float y);
void add_small_explosion_effect(float x, float y);
void add_explosion_effect(float x, float y);
void add_eri_regen(float x, float y);
void add_eri_dead(float x, float y);
#endif /* INC_EFFECT_H_ */
