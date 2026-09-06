/*
 * input.c
 *
 *  Created on: Apr 12, 2026
 *      Author: PC
 */

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "input.h"

extern ADC_HandleTypeDef hadc3;
extern UART_HandleTypeDef huart1;

static volatile uint16_t input_XY[2] __attribute__((aligned(32)));
input_t input_other;

void input_init() {
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*) input_XY, 2);
	//HAL_UART_Receive_DMA(&huart1, (uint8_t*)&input_2p, 1);
}

void input_init_2p() {
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*) input_XY, 2);
}

input_t get_my_input() {
	input_t input = { 0, }; //0000 0000 (나중에 추가할 버튼4개, 우좌하상)

	if (input_XY[0] > 3500) { //우
		input.right = 1;
	} else if (input_XY[0] < 300) { //좌
		input.left = 1;
	}

	if (input_XY[1] > 3500) { //하
		input.down = 1;
	} else if (input_XY[1] < 300) { //상
		input.up = 1;
	}

	if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_RESET) //JUMP
	{
		input.shot = 1;
	}

	if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_4) == GPIO_PIN_RESET) //SHOT
	{
		input.jump = 1;
	}

	return input;
}

input_t get_other_input() {
	return input_other;
}


void set_other_input(input_t input) {
	input_other = input;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC3) {
		SCB_InvalidateDCache_by_Addr((uint32_t*) input_XY, sizeof(input_XY));
	}
}
