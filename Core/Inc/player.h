
#ifndef INC_PLAYER_H_
#define INC_PLAYER_H_

#include <stdint.h>
#include "object.h"
#include "texture.h"
#include "input.h"
#include "health.h"

#define PLAYER_INIT0_OBJECT { 240.0f, 240.0f, 32, 32 }
#define PLAYER_INIT1_OBJECT { 980.0f, 240.0f, 32, 32 }
#define PLAYER_INIT_STATE { 1, 0, 0, 0,  0, 0, 0, 1,  0 }
#define PLAYER_INIT_HEALTH (255)
#define PLAYER_INIT_DAMAGE (10)
#define PLAYER_IDLE_TEXTURE_TOP_RIGHT { image_data_eri_idle_top_right_192_28_6frame, 192, 0, 200, 32, 28, 0, 6, 1 }
#define PLAYER_IDLE_TEXTURE_TOP_LEFT { image_data_eri_idle_top_letf_192_28_6frame, 192, 0, 200, 32, 28, 0, 6, 1 }
#define PLAYER_IDLE_TEXTURE_BOTTOM_RIGHT { image_data_eri_idle_bottom_right_20_24_1frame, 20, 0, 200, 20, 24, 0, 1, 1}
#define PLAYER_IDLE_TEXTURE_BOTTOM_LEFT { image_data_eri_idle_bottom_left_20_24_1frame, 20, 0, 200, 20, 24, 0, 1, 1 }
#define PLAYER_MOVE_TEXTURE_BOTTOM_RIGHT { image_data_eri_move_bottom_right_156_26_6frame, 156, 0, 200, 26, 26, 0, 6, 1 }
#define PLAYER_MOVE_TEXTURE_BOTTOM_LEFT { image_data_eri_move_bottom_left_156_26_6frame, 156, 0, 200, 26, 26, 0, 6, 1 }
#define PLAYER_JUMP_TEXTURE_BOTTOM_RIGHT { image_data_eri_jump_bottom_right_144_30_6frame, 144, 0, 200, 24, 30, 0, 6, 0 }
#define PLAYER_JUMP_TEXTURE_BOTTOM_LEFT { image_data_eri_jump_bottom_left_144_30_6frame, 144, 0, 200, 24, 30, 0, 6, 0 }
#define PLAYER_SHOT_TEXTURE_TOP_RIGHT { image_data_eri_shot_top_right_222_25_6frame, 222, 0, 100, 37,25, 0, 6, 0 }
#define PLAYER_SHOT_TEXTURE_TOP_LEFT { image_data_eri_shot_top_left_222_25_6frame, 222, 0, 100, 37,25, 0, 6, 0 }
#define PLAYER_SHOT_UP_TEXTURE_TOP_RIGHT { image_data_eri_shot_up_top_right_180_37_6frame, 180, 0, 200, 30, 37, 0, 6, 0 }
#define PLAYER_SHOT_UP_TEXTURE_TOP_LEFT { image_data_eri_shot_up_top_left_180_37_6frame, 180, 0, 200, 30, 37, 0, 6, 0 }
#define PLAYER_SHOT_DOWN_TEXTURE_TOP_RIGHT { image_data_eri_shot_down_top_right_132_42_6frame, 132, 0, 200, 22, 42, 0, 6, 0 }
#define PLAYER_SHOT_DOWN_TEXTURE_TOP_LEFT { image_data_eri_shot_down_top_left_132_42_6frame, 132, 0, 200, 22, 42, 0, 6, 0 }

#define PLAYER_HEALTH_BAR { image_data_health_32_4, 32, 0, 200, 32, 4, 0, 1, 0 }
#define PLAYER_HEALTH_BAR_EMPTY { image_data_health_empty_32_4, 32, 0, 200, 32, 4, 0, 1, 0 }
#define PLAYER_HEALTH_BAR_ANI { image_data_health_animation_32_4, 32, 0, 150, 32, 4, 0, 1, 0 }
#define PLAYER_HEALTH_BAR_OFFSET_Y (-6)

#define ERI_IDLE_TOP_RIGHT (0x9033cb20)
#define ERI_IDLE_TOP_LEFT (0x9033f520)
#define ERI_SHOT_TOP_RIGHT (0x9034a220)
#define ERI_SHOT_TOP_LEFT (0x9034cd80)
#define ERI_SHOT_UP_RIGHT (0)
#define ERI_SHOT_UP_LEFT (1)
#define ERI_SHOT_DOWN_RIGHT (2)
#define ERI_SHOT_DOWN_LEFT (3)
#define ERI_MOVE_RIGHT (0x90341f20)
#define ERI_MOVE_LEFT (0x90343ee0)

enum {
	IDLE = 0x1,
	MOVE = 0x2,
	JUMP = 0x4,
	SHOT = 0x08,

	LOOK_UP = 0x10,
	LOOK_DOWN = 0x20,
	LOOK_LEFT = 0x40,
	LOOK_RIGHT = 0x80,

	HIT = 0x100
};

typedef struct {
	uint16_t idle : 1;
	uint16_t move : 1;
	uint16_t jump : 1;
	uint16_t shot : 1;

	uint16_t look_up : 1;
	uint16_t look_down : 1;
	uint16_t look_left : 1;
	uint16_t look_right : 1;

	uint16_t hit : 1;
} state_t;

typedef struct {
	uint8_t active;
	object_t object;
	texture_t top;
	texture_t bottom;
	state_t state;
	health_t health;
	uint16_t score;
	uint8_t damage;
	short regen_time;
	float v_y;
} __attribute__((aligned(32))) player_t;


void player_update(uint8_t index, input_t input, uint32_t delta_time);
void player_render(uint8_t index, uint32_t delta_time);

static void update_state(uint8_t index, input_t input);
static void health_bar_render(uint8_t index, uint32_t delta_time);

#endif
