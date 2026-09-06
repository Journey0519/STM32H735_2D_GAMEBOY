/*
 * health.h
 *
 *  Created on: Apr 24, 2026
 *      Author: PC
 */

#ifndef INC_HEALTH_H_
#define INC_HEALTH_H_

#include <stdint.h>
#include "texture.h"

typedef struct {
	short health;
	texture_t health_bar_empty;
	texture_t health_bar_ani;
	texture_t health_bar;
}  health_t;


#endif /* INC_HEALTH_H_ */
