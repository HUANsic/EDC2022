/*
 * delay_us.h
 *
 *  Created on: Feb 4, 2022
 *      Author: Zonghuan Wu
 */

#ifndef __HUANSIC_DELAY_US__
#define __HUANSIC_DELAY_US__

#include "stm32f3xx_hal.h"

void delay_init(TIM_HandleTypeDef *htim);

void delay_us(uint16_t duration);

#endif /* __HUANSIC_DELAY_US__ */
