/*
 * texture.h
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */

#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include <resource/texture/background_texture.h>
#include <resource/texture/bullet_effect_texture.h>
#include <resource/texture/car_texture.h>
#include <resource/texture/effect_texture.h>
#include <resource/texture/eri_texture.h>
#include <resource/texture/flying_texture.h>
#include <resource/texture/item_texture.h>
#include <resource/texture/octopus_texture.h>
#include <resource/texture/start_screen_texture.h>
#include <resource/texture/ui_texture.h>

typedef struct {
	uint32_t* p_texture; //렌더할 텍스쳐주소
	uint16_t total_frame_width;
	uint16_t accum_dt; //delta time accumulation 애니메이션 어디까지 진행됐나
	uint16_t next_dt; //언제 다음 프레임으로 넘어가나
	uint16_t frame_width;
	uint16_t frame_height;
	uint8_t cur_frame;   //현재 프레임
	uint8_t max_frame_count; //총 몇 프레임
	uint8_t isloop; //다시 반복?
} texture_t;

#endif /* INC_TEXTURE_H_ */
