/*
 * sound.h
 *
 *  Created on: May 22, 2026
 *      Author: PC
 */

#ifndef INC_SOUND_H_
#define INC_SOUND_H_

#include <stdint.h>

typedef struct {
	uint8_t isActive;
	uint8_t* address;
	uint32_t size;
	uint32_t PlaybackPosition;
} sfx_t;

void play_buff_init();
void update_sound();
void add_shot_sound();
void add_jump_sound();

void add_start_sound();
void add_eri_dead_sound();
void add_drop_sound();
void add_boom_sound();
void add_explosion_sound();
void add_fish_sound();
void add_coin_sound();

#endif /* INC_SOUND_H_ */
