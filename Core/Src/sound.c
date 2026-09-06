#include <resource/sound/bgm_sound.h>
#include <resource/sound/jump_sound.h>
#include <resource/sound/reload_sound.h>
#include <resource/sound/shot_sound.h>
#include "resource/sound/boom_sound.h"
#include "resource/sound/drop_sound.h"
#include "resource/sound/eri_dead_sound.h"
#include "resource/sound/explosion_sound.h"
#include "resource/sound/start_sound.h"
#include "resource/sound/coin_sound.h"
#include "resource/sound/fish_sound.h"

#include "sound.h"

#include "stm32h7xx_hal.h"

#define PLAY_BUFF_SIZE (4096)
#define PLAY_HEADER (44)

#define SFX_UPDATE(index) \
	mixed += ((*(int16_t *)(sfxs[index].address + sfxs[index].PlaybackPosition)) << 2) * sfxs[index].isActive; \
	sfxs[index].PlaybackPosition += 2; \

#define SFX_END_CHECK(index) \
		if((sfxs[index].PlaybackPosition + PLAY_BUFF_SIZE/2) > sfxs[index].size) { \
			sfxs[index].isActive = 0; \
			sfxs[index].address = BGM_SOUND_ADDR; \
			sfxs[index].PlaybackPosition = 44; \
		} \

extern SAI_HandleTypeDef hsai_BlockB1;

int16_t PlayBuff[4096] __attribute__((aligned(32)));
uint32_t PlaybackPosition = 0;
int16_t UpdatePointer = -1;

sfx_t sfxs[10] = { 0, };


void play_buff_init() {
	for(int i=0; i < PLAY_BUFF_SIZE; i++)
	{
		PlayBuff[i]= *((uint16_t*)(BGM_SOUND_ADDR + 44 + (i*2)));
	}

	if(HAL_OK != HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)PlayBuff, 4096))
	{
		Error_Handler();
	}

	PlaybackPosition   = PLAY_BUFF_SIZE + PLAY_HEADER;

	for (int i = 0; i < 10; i++) {
		sfxs[i].PlaybackPosition = 0;
		sfxs[i].address = BGM_SOUND_ADDR;
		sfxs[i].isActive = 0;
		sfxs[i].size = 0;
	}
}

void update_sound() {
	if (UpdatePointer != -1) {
		int position = UpdatePointer;
		int32_t mixed = 0;
		UpdatePointer = -1;

		/* Update the first or the second part of the buffer */
		for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
		{
			mixed = (*(int16_t *)(BGM_SOUND_ADDR + PlaybackPosition));
			PlaybackPosition += 2;

			SFX_UPDATE(0);
			SFX_UPDATE(1);
			SFX_UPDATE(2);
			SFX_UPDATE(3);
			SFX_UPDATE(4);
			SFX_UPDATE(5);
			SFX_UPDATE(6);
			SFX_UPDATE(7);
			SFX_UPDATE(8);
			SFX_UPDATE(9);

			mixed = mixed > 32767 ? 32767 : mixed;
			mixed = mixed < -32768 ? -32768 : mixed;
			PlayBuff[i+position] = mixed;
		}

		/* Clean Data Cache to update the content of the SRAM */
		SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuff[position], 2048 * 2);

		/* check the end of the file */
		if((PlaybackPosition + PLAY_BUFF_SIZE/2) > 22 * 1024 * 1024)
		{
			PlaybackPosition = PLAY_HEADER;
		}

		SFX_END_CHECK(0);
		SFX_END_CHECK(1);
		SFX_END_CHECK(2);
		SFX_END_CHECK(3);
		SFX_END_CHECK(4);
		SFX_END_CHECK(5);
		SFX_END_CHECK(6);
		SFX_END_CHECK(7);
		SFX_END_CHECK(8);
		SFX_END_CHECK(9);
	}
}


void add_shot_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = shot_sound;
			sfxs[i].size = 117944;

			return;
		}
	}
}

void add_jump_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = jump_sound;
			sfxs[i].size = 24012;

			return;
		}
	}
}

void add_start_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = start_sound;
			sfxs[i].size = 218116;

			return;
		}
	}
}

void add_eri_dead_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = eri_dead_sound;
			sfxs[i].size = 74972;

			return;
		}
	}
}

void add_drop_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = drop_sound;
			sfxs[i].size = 61144;

			return;
		}
	}
}

void add_boom_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = boom_sound;
			sfxs[i].size = 170156;

			return;
		}
	}
}

void add_explosion_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = explosion_sound;
			sfxs[i].size = 117944;

			return;
		}
	}
}

void add_fish_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = fish_sound;
			sfxs[i].size = 154396;

			return;
		}
	}
}

void add_coin_sound() {
	for (int i = 0; i < 10; i++) {
		if (sfxs[i].isActive == 0) {
			sfxs[i].isActive = 1;
			sfxs[i].PlaybackPosition = PLAY_HEADER;
			sfxs[i].address = coin_sound;
			sfxs[i].size = 109032;

			return;
		}
	}
}


void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  UpdatePointer = PLAY_BUFF_SIZE/2;
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  UpdatePointer = 0;
}
