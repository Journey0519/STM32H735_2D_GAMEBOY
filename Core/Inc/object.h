/*
 * object.h
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */

#ifndef INC_OBJECT_H_
#define INC_OBJECT_H_

#include <stdint.h>

typedef struct {
	float position_x; //왼쪽아래 기준
	float position_y;
	uint8_t width;       //차지하는 사각형 너비
	uint8_t hegiht;
} object_t;


#endif /* INC_OBJECT_H_ */
