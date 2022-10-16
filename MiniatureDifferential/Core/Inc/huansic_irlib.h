/*
 * huansic_irlib.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_IRLIB_H_
#define INC_HUANSIC_IRLIB_H_

#include "stm32f1xx_hal.h"

typedef enum {
	IR_BOTH_OFF = 0,
	IR_OUTTER_ON = 1,
	IR_INNER_ON = 2,
	IR_BOTH_ON = 3
} IR_State;		// by bits

typedef struct {
	GPIO_TypeDef *outter_port, *inner_port;
	uint16_t outter_pin, inner_pin;
} IR_HandleTypeDef;

inline void huansic_ir_on() {
	return HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
}

inline void huansic_ir_off() {
	return HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
}

inline IR_State huansic_ir_check(IR_HandleTypeDef *hir) {
	return (HAL_GPIO_ReadPin(hir->outter_port, hir->outter_pin) ? 1 : 0)
			| (HAL_GPIO_ReadPin(hir->inner_port, hir->inner_pin) ? 2 : 0);
}

#endif /* INC_HUANSIC_IRLIB_H_ */
