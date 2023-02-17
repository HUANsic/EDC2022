/*
 * huansic_jy62lib.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_JY62LIB_H_
#define INC_HUANSIC_JY62LIB_H_

#include "huansic_types.h"
#include <math.h>

typedef enum{
	JY62_OK = HAL_OK,
	JY62_ERROR = HAL_ERROR,
	JY62_BUDY = HAL_BUSY,
	JY62_TIMEOUT = HAL_TIMEOUT,
	JY62_NULLPTR = 4,
	JY62_WRONG_HEADER,
	JY62_CHECKSUM_FAIL
} JY62_StatusTypeDef;

typedef struct {
	UART_HandleTypeDef *uartPort;

	float accel_x, accel_y, accel_z;	// 4 + 4 + 4
	float omega_x, omega_y, omega_z;	// 4 + 4 + 4
	float theta_x, theta_y, theta_z;	// 4 + 4 + 4

	uint32_t lastUpdated;				// 4

	uint8_t buffer[11];		// put at the end to prevent block alignment issues
} JY62_HandleTypeDef;

JY62_StatusTypeDef huansic_jy62_init(JY62_HandleTypeDef *hjy62);

JY62_StatusTypeDef huansic_jy62_resetAngle(JY62_HandleTypeDef *hjy62);

JY62_StatusTypeDef huansic_jy62_decodePackage(JY62_HandleTypeDef *hjy62);

JY62_StatusTypeDef huansic_jy62_setDMA(JY62_HandleTypeDef *hjy62);

JY62_StatusTypeDef huansic_jy62_align(JY62_HandleTypeDef *hjy62, uint16_t timeout);

#endif /* INC_HUANSIC_JY62LIB_H_ */
