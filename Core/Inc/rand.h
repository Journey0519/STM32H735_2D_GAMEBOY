/*
 * rand.h
 *
 *  Created on: Jun 13, 2026
 *      Author: PC
 */

#ifndef INC_RAND_H_
#define INC_RAND_H_

#include <stdint.h>

void SetRandomSeed(uint32_t seed);
uint32_t FastRand(void);

#endif /* INC_RAND_H_ */
