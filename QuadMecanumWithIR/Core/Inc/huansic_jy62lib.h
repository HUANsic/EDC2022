/*
 * huansic_jy62lib.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_JY62LIB_H_
#define INC_HUANSIC_JY62LIB_H_

#include "stm32f3xx_hal.h"
#include <assert.h>
#include <math.h>

typedef struct {
	UART_HandleTypeDef *uartPort;

	uint8_t buffer[11];

	float accel_x, accel_y, accel_z;
	float omega_x, omega_y, omega_z;
	float theta_x, theta_y, theta_z;

	float temperature;

	uint32_t lastUpdated;
} JY62_HandleTypeDef;

void huansic_jy62_init(JY62_HandleTypeDef *hjy62);

void huansic_jy62_resetAngle(JY62_HandleTypeDef *hjy62);

void huansic_jy62_decodePackage(JY62_HandleTypeDef *hjy62);

void huansic_jy62_setDMA(JY62_HandleTypeDef *hjy62);

#endif /* INC_HUANSIC_JY62LIB_H_ */
