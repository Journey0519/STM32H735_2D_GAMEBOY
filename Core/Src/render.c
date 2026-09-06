/*
 * render.c
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */
#include "render.h"
#include "stm32h7xx_hal.h"

#define BG_WIDTH (1654)
#define LCD_WIDTH (480)
#define LCD_HEIGHT (272)
#define LCD_DISP_Pin GPIO_PIN_10

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

static uint16_t framebuffer0[480 * 272] __attribute__((section(".framebuffer_section"), aligned(32)));
static uint16_t framebuffer1[480 * 272] __attribute__((section(".hyperram_data"), aligned(32)));
static uint16_t *framebuffers[2] = { framebuffer0, framebuffer1 };
static uint32_t back_buffer_idx = 0;

float camera_offset[2] = { 0.0f, 0.0f };
uint8_t my_camera_index = 0;

void render_init() {
	HAL_LTDC_SetAddress(&hltdc, (uint32_t) back_background, 0);
	HAL_LTDC_SetAddress(&hltdc, (uint32_t) framebuffers[0], 1);
	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LCD_DISP_Pin, GPIO_PIN_SET);
}

void render_bg() {
	hdma2d.LayerCfg[1].InputOffset = BG_WIDTH - LCD_WIDTH;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB4444;
	hdma2d.Init.Mode = DMA2D_M2M;
	hdma2d.Init.OutputOffset = 0;

	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_Init(&hdma2d);

	HAL_DMA2D_Start(&hdma2d,
			(uint32_t) (background + (int) camera_offset[my_camera_index]),
			(uint32_t) (framebuffers[back_buffer_idx]),
			480, 272);
}

void render_texture(float x, float y, float offset_x, float offset_y, texture_t* p_texture, uint32_t delta_time) {
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB4444;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB4444;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB4444;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.Init.Mode = DMA2D_M2M_BLEND;

	int width = p_texture->frame_width;
	int height = p_texture->frame_height;
	float start_x = x + offset_x;
	float start_y = y + offset_y;         //왼쪽 위의 점
	uint32_t start_x_offset = 0;
	uint32_t start_y_offset = 0;
	uint16_t screen_x;
	uint16_t screen_y;

	if (start_x + p_texture->frame_width > camera_offset[my_camera_index] + LCD_WIDTH) {
		width = ((camera_offset[my_camera_index] + LCD_WIDTH) - (start_x));
	} else if ( start_x < camera_offset[my_camera_index]) {
		start_x_offset =  ((camera_offset[my_camera_index]) - (start_x));
		width = ((start_x + p_texture->frame_width) - (camera_offset[my_camera_index]));
	}

	if (start_y + p_texture->frame_height > LCD_HEIGHT) {
		height = ((LCD_HEIGHT) - (start_y));
	} else if (start_y < 0) {
		start_y_offset =  (-start_y);
		height = (start_y + p_texture->frame_height);
	}

	if (width <= 0 || height <= 0) {
		goto texture_update;
		return;
	}

	screen_x = (int)(start_x - camera_offset[my_camera_index]) < 0 ? 0 : (int)(start_x - camera_offset[my_camera_index]);
	screen_y = start_y < 0 ? 0 : start_y;

	hdma2d.LayerCfg[0].InputOffset = LCD_WIDTH - width;
	hdma2d.LayerCfg[1].InputOffset = p_texture->total_frame_width - width;
	hdma2d.Init.OutputOffset = LCD_WIDTH - width;

	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
	HAL_DMA2D_ConfigLayer(&hdma2d, 0);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_Init(&hdma2d);
	HAL_DMA2D_BlendingStart(&hdma2d,
			(uint32_t) ((uint16_t*)p_texture->p_texture + (start_x_offset + start_y_offset * p_texture->frame_width)  + p_texture->frame_width * p_texture->cur_frame),
			(uint32_t) (framebuffers[back_buffer_idx] + (screen_x)  + ((screen_y)  * LCD_WIDTH)),
			(uint32_t) (framebuffers[back_buffer_idx] + (screen_x) + ((screen_y)  * LCD_WIDTH)),
			width, height);

texture_update:
	if (p_texture->accum_dt > p_texture->next_dt) {
		p_texture->accum_dt = 0;
		p_texture->cur_frame++;
		if (p_texture->cur_frame >= p_texture->max_frame_count) {
			p_texture->cur_frame = p_texture->isloop ? 0 : p_texture->max_frame_count - 1;
		}
	} else {
		p_texture->accum_dt += delta_time;
	}

	return;
}

void render_texture_no_blend(float x, float y, float offset_x, float offset_y, texture_t* p_texture, uint32_t delta_time) {
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB4444;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB4444;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB4444;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.Init.Mode = DMA2D_M2M;

	int width = p_texture->frame_width;
	int height = p_texture->frame_height;
	float start_x = x + offset_x;
	float start_y = y + offset_y;         //왼쪽 위의 점
	uint32_t start_x_offset = 0;
	uint32_t start_y_offset = 0;
	uint16_t screen_x;
	uint16_t screen_y;

	if (start_x + p_texture->frame_width > camera_offset[my_camera_index] + LCD_WIDTH) {
		width = ((camera_offset[my_camera_index] + LCD_WIDTH) - (start_x));
	} else if ( start_x < camera_offset[my_camera_index]) {
		start_x_offset =  ((camera_offset[my_camera_index]) - (start_x));
		width = ((start_x + p_texture->frame_width) - (camera_offset[my_camera_index]));
	}

	if (start_y + p_texture->frame_height > LCD_HEIGHT) {
		height = ((LCD_HEIGHT) - (start_y));
	} else if (start_y < 0) {
		start_y_offset =  (-start_y);
		height = (start_y + p_texture->frame_height);
	}

	if (width <= 0 || height <= 0) {
		return;
	}

	screen_x = (int)(start_x - camera_offset[my_camera_index]) < 0 ? 0 : (int)(start_x - camera_offset[my_camera_index]);
	screen_y = start_y < 0 ? 0 : start_y;

	hdma2d.LayerCfg[0].InputOffset = LCD_WIDTH - width;
	hdma2d.LayerCfg[1].InputOffset = p_texture->total_frame_width - width;
	hdma2d.Init.OutputOffset = LCD_WIDTH - width;

	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
	HAL_DMA2D_ConfigLayer(&hdma2d, 0);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_Init(&hdma2d);
	HAL_DMA2D_Start(&hdma2d,
			(uint32_t) ((uint16_t*)p_texture->p_texture + (start_x_offset + start_y_offset * p_texture->frame_width)  + p_texture->frame_width * p_texture->cur_frame),
			(uint32_t) (framebuffers[back_buffer_idx] + (screen_x)  + ((screen_y)  * LCD_WIDTH)),
			width, height);

	if (p_texture->accum_dt > p_texture->next_dt) {
		p_texture->accum_dt = 0;
		p_texture->cur_frame++;
		if (p_texture->cur_frame >= p_texture->max_frame_count) {
			p_texture->cur_frame = p_texture->isloop ? 0 : p_texture->max_frame_count - 1;
		}
	} else {
		p_texture->accum_dt += delta_time;
	}
}


void swap_framebuffer() {
	HAL_LTDC_SetAddress(&hltdc, (uint32_t) framebuffers[back_buffer_idx],
			1);
	HAL_LTDC_Reload(&hltdc, LTDC_VERTICALSYNC);
	back_buffer_idx = !back_buffer_idx;
}


void render_hitbox(object_t object) {
	hdma2d.Init.Mode = DMA2D_R2M;


	int width = object.width;
	int height = object.hegiht;
	float start_x = object.position_x;
	float start_y = object.position_y - height;         //왼쪽 위의 점

	uint16_t screen_x;
	uint16_t screen_y;

	if (start_x + width > camera_offset[my_camera_index] + LCD_WIDTH) {
		width = ((camera_offset[my_camera_index] + LCD_WIDTH) - (start_x));
	} else if ( start_x < camera_offset[my_camera_index]) {
		width = ((start_x + width) - (camera_offset[my_camera_index]));
	}

	if (start_y + height > LCD_HEIGHT) {
		height = ((LCD_HEIGHT) - (start_y));
	} else if (start_y < 0) {
		height = (start_y + height);
	}

	if (width <= 0 || height <= 0) {
		return;
	}

	screen_x = (int)(start_x - camera_offset[my_camera_index]) < 0 ? 0 : (int)(start_x - camera_offset[my_camera_index]);
	screen_y = start_y < 0 ? 0 : start_y;

	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.Init.OutputOffset = LCD_WIDTH - width;

	uint32_t color = 0;
	uint8_t r = 0; //(uint8_t)((0xFF * (float)(1000 - object.test_health) / 1000 + 0x00 * (float)object.test_health / 1000));
	uint8_t g = 0xFF; //(uint8_t)((0x00 * (float)(1000 - object.test_health) / 1000 + 0xFF * (float)object.test_health / 1000));
	color = (0xFF << 24) | (r << 16) | (g << 8) | 0x00;

	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_Init(&hdma2d);
	HAL_DMA2D_Start(&hdma2d,
			(uint32_t) (color),
			(uint32_t) (framebuffers[back_buffer_idx] + (screen_x)  + ((screen_y)  * LCD_WIDTH)),
			width, height);
}

void render_box_R2M(float x, float y, float offset_x, float offset_y, uint32_t width, uint32_t height, uint32_t color, uint32_t delta_time) {
	hdma2d.Init.Mode = DMA2D_R2M;

	float start_x = x + offset_x;
	float start_y = y + offset_y;

	uint16_t screen_x;
	uint16_t screen_y;

	if (start_x + width > camera_offset[my_camera_index] + LCD_WIDTH) {
		width = ((camera_offset[my_camera_index] + LCD_WIDTH) - (start_x));
	} else if ( start_x < camera_offset[my_camera_index]) {
		width = ((start_x + width) - (camera_offset[my_camera_index]));
	}

	if (start_y + height > LCD_HEIGHT) {
		height = ((LCD_HEIGHT) - (start_y));
	} else if (start_y < 0) {
		height = (start_y + height);
	}

	if (width <= 0 || height <= 0) {
		return;
	}

	screen_x = (int)(start_x - camera_offset[my_camera_index]) < 0 ? 0 : (int)(start_x - camera_offset[my_camera_index]);
	screen_y = start_y < 0 ? 0 : start_y;

	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.Init.OutputOffset = LCD_WIDTH - width;

	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	HAL_DMA2D_Init(&hdma2d);
	HAL_DMA2D_Start(&hdma2d,
			(uint32_t) (color),
			(uint32_t) (framebuffers[back_buffer_idx] + (screen_x)  + ((screen_y)  * LCD_WIDTH)),
			width, height);
}
