/*
 * huansic_motorlib.h
 *
 *  Created on: Sep 27, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_MOTORLIB_H_
#define INC_HUANSIC_MOTORLIB_H_

#include <assert.h>
#include <math.h>
#include "stm32f1xx_hal.h"

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

void huansic_motor_init(Motor_HandleTypeDef *hmotor);

void huansic_motor_pid(Motor_HandleTypeDef *hmotor);

#endif /* INC_HUANSIC_MOTORLIB_H_ */
