/*
 * ui.h
 *
 *  Created on: Apr 24, 2026
 *      Author: PC
 */

#ifndef INC_UI_TEXTURE_H_
#define INC_UI_TEXTURE_H_

#include <stdint.h>

#define HEALTH_OFFSET_Y (-6)
#define GO_DOWN_INIT { image_data_go_down_512_32_16p, 512, 0, 100, 32, 32, 0, 16, 1 }

extern const uint16_t image_data_health_empty_32_4[128];
extern const uint16_t image_data_health_animation_32_4[128];
extern const uint16_t image_data_health_32_4[128];
extern const uint16_t image_data_go_down_512_32_16p[16384];
extern const uint16_t image_data_loading_300_81_1[24300];
extern const uint16_t image_data_loading_bar_265_47[12455];
extern const uint16_t num_font_160_16[2560];
extern const uint16_t image_data_1p_288_37_8f[10656];
extern const uint16_t image_data_2p_296_37_8f[10952];
extern const uint16_t image_data_counter_300_54_6f[16200];
#endif /* INC_UI_TEXTURE_H_ */
