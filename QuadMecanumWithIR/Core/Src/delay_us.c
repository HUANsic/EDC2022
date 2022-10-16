/*
 * delay_us.c
 *
 *  Created on: Feb 4, 2022
 *      Author: Zonghuan Wu
 */
#include "delay_us.h"

uint32_t GLOBAL_MILLIS;

TIM_HandleTypeDef *huansic_delayTimer;

void delay_init(TIM_HandleTypeDef *htim) {
	huansic_delayTimer = htim;
	GLOBAL_MILLIS = 0;
	HAL_TIM_Base_Start(huansic_delayTimer);
}

void delay_us(uint16_t duration) {
	uint32_t tick = huansic_delayTimer->Instance->CNT + GLOBAL_MILLIS * 1000;
	while (GLOBAL_MILLIS * 1000 + huansic_delayTimer->Instance->CNT - tick < duration)
		;
}

uint32_t get_us_tick() {
	return huansic_delayTimer->Instance->CNT;
}
