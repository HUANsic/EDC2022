/*
 * huansic_types.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_TYPES_H_
#define INC_HUANSIC_TYPES_H_

#include "stm32f1xx_hal.h"

typedef struct {
	uint16_t x, y;
} Coordinate;

typedef struct {
	float x, y;
} fCoordinate;

typedef struct {
	Coordinate startCoord, destCoord;		// 4 + 4
	uint32_t timeLimit;						// 4
	uint32_t startTime;						// 4
	uint8_t reward;							// 1 (4)
} Order;									// = 20

typedef struct {
	Coordinate coord1, coord2;				// 4 + 4
} Rectangle;								// = 8

typedef struct {
	TIM_HandleTypeDef *counter;
	TIM_HandleTypeDef *posTimer, *negTimer;
	uint32_t pos_channel, neg_channel;		// TIM_CHANNEL_n
	float kp, ki, kd;
	float dt;		// Feedback Control Period; used to perform the calculation
	// uint16_t maxCount;			// divider used to normalize the error; just assume it is 16 bit
	// uint16_t timerPeriod;		// multiplier used to convert the normalized output into timer output; get this from 'timer'
	uint32_t lastTick;
	float lastError;
	float lastSpeed, last5Speed;
	float sumError;
	float goalSpeed;
} Motor_HandleTypeDef;

typedef struct {
	UART_HandleTypeDef *uartPort;
	DMA_HandleTypeDef *rxDMA;

	float accel_x, accel_y, accel_z;
	float omega_x, omega_y, omega_z;
	float theta_x, theta_y, theta_z;

	uint32_t lastUpdated;

	uint8_t buffer[11];		// put at the end to prevent block alignment issues
} JY62_HandleTypeDef;

typedef struct {
	UART_HandleTypeDef *uartPort;
	DMA_HandleTypeDef *rxDMA;

	uint32_t lastUpdated;
	uint8_t checkedHeader;
	uint8_t msgType;

	// for internal use; user shall not modify this
	uint8_t nextPackageID;			// 0x00: next is header; others: check ID
	uint8_t nextPackageLength;		// length of next DMA receive

	// uint8_t buffer[max(1 + 8 + 8 + 8 + 4 + (4 + 5 * 32) + 32, 1 + (4 + 5 * 16) + 8 + 2 * (4 + (3 * 8))];
	// uint8_t buffer[max(225, 149)];
	uint8_t buffer[225];	// put at the end to prevent block alignment issues
} XB_HandleTypeDef;

typedef struct Path_t{
	Coordinate start, end;		// 4 + 4
	float speed;				// 2
	float radius, omega;		// 2 + 2
	enum Type{
		ignore = 0,
		linear = 1,
		angular = 2
	} type;						// 1
	uint8_t referenceCount;		// 1
	struct Path_t *nextPath;	// 4
} Path;

#endif /* INC_HUANSIC_TYPES_H_ */
