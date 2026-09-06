/*
 * input.h
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

enum {
	BT_UP = 0x01,
	BT_DOWN = 0x02,
	BT_LEFT = 0x04,
	BT_RIGHT = 0x08,
	BT_SHOT = 0x10,
	BT_JUMP = 0x20
};

typedef struct {
	uint8_t up : 1;
	uint8_t down : 1;
	uint8_t left : 1;
	uint8_t right : 1;
	uint8_t shot : 1;
	uint8_t jump : 1;
	uint8_t null0 : 1;
	uint8_t null1 : 1;
} input_t;

void input_init();
void input_init_2p();

input_t get_my_input();
input_t get_other_input();

void set_other_input(input_t input);

#endif /* INC_INPUT_H_ */
