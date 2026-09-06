/*
 * eri.h
 *
 *  Created on: Apr 10, 2026
 *      Author: PC
 */

#ifndef INC_ERI_TEXTURE_H_
#define INC_ERI_TEXTURE_H_

#include <stdint.h>

#define ERI_IDLE_TOP_TOTAL_WIDTH (192)
#define ERI_IDLE_TOP_WIDTH (32)
#define ERI_IDLE_TOP_HEIGHT (28)
#define ERI_IDLE_BOTTOM_TOTAL_WIDTH (20)
#define ERI_IDLE_BOTTOM_WIDTH (20)
#define ERI_IDLE_BOTTOM_HEIGHT (24)
#define ERI_MOVE_BOTTOM_TOTAL_WIDTH (156)
#define ERI_MOVE_BOTTOM_WIDTH (26)
#define ERI_MOVE_BOTTOM_HEIGHT (26)

#define ERI_IDLE_MOVE_BOTTOM_Y_OFFSET (14)
#define ERI_IDLE_MOVE_BOTTOM_RIGHT_X_OFFSET (1)
#define ERI_IDLE_MOVE_BOTTOM_LEFT_X_OFFSET (8)
#define ERI_SHOT_BOTTOM_Y_OFFSET (13)
#define ERI_SHOT_BOTTOM_RIGHT_X_OFFSET (0)
#define ERI_SHOT_BOTTOM_LEFT_X_OFFSET (13)

extern const uint16_t image_data_eri_idle_top_right_192_28_6frame[5376];
extern const uint16_t image_data_eri_idle_top_letf_192_28_6frame[5376];
extern const uint16_t image_data_eri_idle_bottom_right_20_24_1frame[480];
extern const uint16_t image_data_eri_idle_bottom_left_20_24_1frame[480];
extern const uint16_t image_data_eri_move_bottom_right_156_26_6frame[4056];
extern const uint16_t image_data_eri_move_bottom_left_156_26_6frame[4056];
extern const uint16_t image_data_eri_jump_bottom_right_144_30_6frame[4320];
extern const uint16_t image_data_eri_jump_bottom_left_144_30_6frame[4320];
extern const uint16_t image_data_eri_shot_top_right_222_25_6frame[5550];
extern const uint16_t image_data_eri_shot_top_left_222_25_6frame[5550];
extern const uint16_t image_data_eri_shot_up_top_right_180_37_6frame[6660];
extern const uint16_t image_data_eri_shot_up_top_left_180_37_6frame[6660];
extern const uint16_t image_data_eri_shot_down_top_right_132_42_6frame[5544];
extern const uint16_t image_data_eri_shot_down_top_left_132_42_6frame[5544];
extern const uint16_t image_data_eri_dead_440_36_10f[15840];
extern const uint16_t image_data_eri_regen_210_239_6f[50190];
#endif /* INC_ERI_TEXTURE_H_ */
