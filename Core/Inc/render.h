/*
 * render.h
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */
#ifndef INC_RENDER_H_
#define INC_RENDER_H_

#include "texture.h"
#include "object.h"

void render_init();
void render_bg();
extern uint8_t my_camera_index;

void render_texture(float x, float y, float offset_x, float offset_y,
		texture_t* p_texture, uint32_t delta_time);

void render_texture_no_blend(float x, float y, float offset_x, float offset_y,
		texture_t* p_texture, uint32_t delta_time);

void swap_framebuffer();
void render_hitbox(object_t object);

void render_box_R2M(float x, float y, float offset_x, float offset_y,
		uint32_t width, uint32_t height, uint32_t color, uint32_t delta_time);
#endif /* INC_RENDER_H_ */
