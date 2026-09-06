/*
 * rand.c
 *
 *  Created on: Jun 13, 2026
 *      Author: PC
 */

#include "rand.h"

uint32_t game_seed = 1;

void SetRandomSeed(uint32_t seed) {
	if (seed == 0) seed = 1; // 0 방지
	    game_seed = seed;
}


uint32_t FastRand(void) {
	game_seed ^= game_seed << 13;
	    game_seed ^= game_seed >> 17;
	    game_seed ^= game_seed << 5;
	    return game_seed;
}
