/*
 * huansic_motorlib.h
 *
 *  Created on: Sep 27, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_MOTORLIB_H_
#define INC_HUANSIC_MOTORLIB_H_

#include <math.h>
#include "huansic_types.h"

typedef struct {
	TIM_HandleTypeDef *counter;
	TIM_HandleTypeDef *posTimer, *negTimer;
	uint32_t pos_channel, neg_channel;		// (4 + 4) TIM_CHANNEL_n
	float kp, ki, kd;		// 4 + 4 + 4
	float dt;		// (4) Feedback Control Period; used to perform the calculation
	// uint16_t maxCount;			// divider used to normalize the error; just assume it is 16 bit
	// uint16_t timerPeriod;		// multiplier used to convert the normalized output into timer output; get this from 'timer'
	uint16_t lastTick;				// 2 (4)
	float lastError;				// 4
	float lastSpeed, last5Speed;	// 4 + 4
	float sumError;					// 4
	float goalSpeed;				// 4
} Motor_HandleTypeDef;

void huansic_motor_init(Motor_HandleTypeDef *hmotor);

void huansic_motor_pid(Motor_HandleTypeDef *hmotor);

#endif /* INC_HUANSIC_MOTORLIB_H_ */
