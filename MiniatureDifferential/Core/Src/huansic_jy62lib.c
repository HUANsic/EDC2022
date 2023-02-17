/*
 * huansic_jy62lib.c
 *
 *  Created on: Sep 28, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_jy62lib.h"

#define JY62_MESSAGE_LENGTH	11

const uint8_t initAngle_pack[3] = { 0xFF, 0xAA, 0x52 };
const uint8_t calibrateAcce_pack[3] = { 0xFF, 0xAA, 0x67 };
const uint8_t setBaud115200_pack[3] = { 0xFF, 0xAA, 0x63 };
const uint8_t setBaud9600_pack[3] = { 0xFF, 0xAA, 0x64 };
const uint8_t setHorizontal_pack[3] = { 0xFF, 0xAA, 0x65 };
const uint8_t setVertical_pack[3] = { 0xFF, 0xAA, 0x66 };
const uint8_t sleepAndAwake_pack[3] = { 0xFF, 0xAA, 0x60 };

JY62_StatusTypeDef huansic_jy62_init(JY62_HandleTypeDef *hjy62) {
	if (!hjy62)
		return JY62_ERROR;
	if (!hjy62->uartPort)
		return JY62_ERROR;

	huansic_jy62_resetAngle(hjy62);

	hjy62->accel_x = 0;
	hjy62->accel_y = 0;
	hjy62->accel_z = 0;
	hjy62->omega_x = 0;
	hjy62->omega_y = 0;
	hjy62->omega_z = 0;
	hjy62->theta_x = 0;
	hjy62->theta_y = 0;
	hjy62->theta_z = 0;

	hjy62->lastUpdated = HAL_GetTick();

	HAL_UART_Receive_DMA(hjy62->uartPort, hjy62->buffer, JY62_MESSAGE_LENGTH);

	return JY62_OK;
}

JY62_StatusTypeDef huansic_jy62_resetAngle(JY62_HandleTypeDef *hjy62) {
	return HAL_UART_Transmit(hjy62->uartPort, initAngle_pack, 3, HAL_MAX_DELAY);
}

JY62_StatusTypeDef huansic_jy62_decodePackage(JY62_HandleTypeDef *hjy62) {
	// package header
	if (hjy62->buffer[0] != 0x55)
		return JY62_WRONG_HEADER;

	// checksum
	uint8_t sum = hjy62->buffer[0] + hjy62->buffer[1] + hjy62->buffer[2] + hjy62->buffer[3]
			+ hjy62->buffer[4] + hjy62->buffer[5] + hjy62->buffer[6] + hjy62->buffer[7]
			+ hjy62->buffer[8] + hjy62->buffer[9];
	if (hjy62->buffer[10] != sum)
		return JY62_CHECKSUM_FAIL;

	// decoding
	int16_t temp;
	if (hjy62->buffer[1] == 0x51) {		// acceleration
		temp = hjy62->buffer[3];
		temp <<= 8;
		temp |= hjy62->buffer[2];
		hjy62->accel_x = temp / 32768.0 * 16.0 * 9.81;
		temp = hjy62->buffer[5];
		temp <<= 8;
		temp |= hjy62->buffer[4];
		hjy62->accel_y = temp / 32768.0 * 16.0 * 9.81;
		temp = hjy62->buffer[7];
		temp <<= 8;
		temp |= hjy62->buffer[6];
		hjy62->accel_z = temp / 32768.0 * 16.0 * 9.81;
	} else if (hjy62->buffer[1] == 0x52) {		// angular speed
		temp = hjy62->buffer[3];
		temp <<= 8;
		temp |= hjy62->buffer[2];
		hjy62->omega_x = temp / 32768.0 * 2000.0 / 180.0 * M_PI;
		temp = hjy62->buffer[5];
		temp <<= 8;
		temp |= hjy62->buffer[4];
		hjy62->omega_y = temp / 32768.0 * 2000.0 / 180.0 * M_PI;
		temp = hjy62->buffer[7];
		temp <<= 8;
		temp |= hjy62->buffer[6];
		hjy62->omega_z = temp / 32768.0 * 2000.0 / 180.0 * M_PI;
	} else if (hjy62->buffer[1] == 0x53) {		// angle
		temp = hjy62->buffer[3];
		temp <<= 8;
		temp |= hjy62->buffer[2];
		hjy62->theta_x = temp / 32768.0 * M_PI;
		temp = hjy62->buffer[5];
		temp <<= 8;
		temp |= hjy62->buffer[4];
		hjy62->theta_y = temp / 32768.0 * M_PI;
		temp = hjy62->buffer[7];
		temp <<= 8;
		temp |= hjy62->buffer[6];
		hjy62->theta_z = temp / 32768.0 * M_PI;
	}

	// record current time
	hjy62->lastUpdated = HAL_GetTick();

	return JY62_OK;
}

JY62_StatusTypeDef huansic_jy62_align(JY62_HandleTypeDef *hjy62, uint16_t timeout) {
	uint8_t temp;
	uint32_t startTime = HAL_GetTick();
	HAL_UART_Receive(hjy62->uartPort, &temp, 1, 5);
	while (temp != 0x55) {
		HAL_UART_Receive(hjy62->uartPort, &temp, 1, 5);
		if (HAL_GetTick() - startTime > timeout)
			break;
	}
	if (temp == 0x55)
		return JY62_OK;
	return JY62_TIMEOUT;
}

JY62_StatusTypeDef huansic_jy62_setDMA(JY62_HandleTypeDef *hjy62) {
	return HAL_UART_Receive_DMA(hjy62->uartPort, hjy62->buffer, JY62_MESSAGE_LENGTH);
}
