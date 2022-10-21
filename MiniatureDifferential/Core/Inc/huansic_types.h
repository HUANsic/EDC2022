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
	uint16_t x, y;		// 2 + 2
} Coordinate;			// = 4

typedef struct {
	float x, y;			// 4 + 4
} fCoordinate;			// = 4

typedef struct {
	Coordinate startCoord, destCoord;		// 4 + 4
	uint32_t timeLimit;						// 4
	uint32_t startTime;						// 4
	uint8_t reward;							// 1
	int8_t id;								// 1 (3)
} Order;									// = 20

typedef struct {
	Coordinate coord1, coord2;				// 4 + 4
} Rectangle;								// = 8

typedef struct {
	TIM_HandleTypeDef *counter;
	TIM_HandleTypeDef *posTimer, *negTimer;
	uint32_t pos_channel, neg_channel;		// (4 + 4) TIM_CHANNEL_n
	float kp, ki, kd;		// 4 + 4 + 4
	float dt;		// (4) Feedback Control Period; used to perform the calculation
	// uint16_t maxCount;			// divider used to normalize the error; just assume it is 16 bit
	// uint16_t timerPeriod;		// multiplier used to convert the normalized output into timer output; get this from 'timer'
	uint32_t lastTick;				// 4
	float lastError;				// 4
	float lastSpeed, last5Speed;	// 4 + 4
	float sumError;					// 4
	float goalSpeed;				// 4
} Motor_HandleTypeDef;

typedef struct {
	UART_HandleTypeDef *uartPort;
	DMA_HandleTypeDef *rxDMA;

	float accel_x, accel_y, accel_z;	// 4 + 4 + 4
	float omega_x, omega_y, omega_z;	// 4 + 4 + 4
	float theta_x, theta_y, theta_z;	// 4 + 4 + 4

	uint32_t lastUpdated;				// 4

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
	float speed;				// 4
	float radius, omega;		// 4 + 4
	struct Path_t *nextPath;	// 4
	enum Type{
		ignore = 0,
		linear = 1,
		angular = 2
	} type;						// 1
	uint8_t referenceCount;		// 1
} Path;

#endif /* INC_HUANSIC_TYPES_H_ */
