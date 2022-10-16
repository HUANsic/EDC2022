/*
 * huansic_motorlib.c
 *
 *  Created on: Sep 27, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_motorlib.h"

void huansic_motor_init(Motor_HandleTypeDef *hmotor) {
	// checking some stuff
	assert(hmotor->counter);
	assert(hmotor->posTimer);	// the negative channel CAN be NULL
	assert(hmotor->kp);
	assert(hmotor->dt);

	// initialize
	hmotor->lastTick = 0;
	hmotor->lastError = 0;
	hmotor->lastSpeed = 0;
	hmotor->last5Speed = 0;
	hmotor->sumError = 0;
	hmotor->goalSpeed = 0;
	hmotor->counter->Instance->CNT = 0;

	// shut down the motor for now
	*(&hmotor->posTimer->Instance->CCR1 + hmotor->pos_channel) = 0;
	if (hmotor->negTimer)
		*(&hmotor->negTimer->Instance->CCR1 + hmotor->neg_channel) = 0;

	// and start the counter and timer
	HAL_TIM_Encoder_Start(hmotor->counter, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Start(hmotor->posTimer, hmotor->pos_channel);
	if (hmotor->negTimer)
		HAL_TIM_PWM_Start(hmotor->negTimer, hmotor->neg_channel);
}

void huansic_motor_pid(Motor_HandleTypeDef *hmotor) {
	uint32_t diffTick = hmotor->counter->Instance->CNT - hmotor->lastTick;

	hmotor->lastSpeed = (float) diffTick / hmotor->dt;
	hmotor->last5Speed = (4.0 * hmotor->last5Speed + hmotor->lastSpeed) / 5.0;

	// Derivative
	float dError = hmotor->lastError - (hmotor->goalSpeed - hmotor->last5Speed);

	// Proportional
	hmotor->lastError = hmotor->goalSpeed - hmotor->last5Speed;

	// Integral
	hmotor->sumError += hmotor->lastError;

	// calculate and constrain the duty cycle
	float foutput = hmotor->kp * hmotor->lastError + hmotor->ki * hmotor->sumError * hmotor->dt
			+ hmotor->kd * dError / hmotor->dt;
	foutput = foutput > 1.0 ? 1.0 : (foutput < -1.0 ? -1.0 : foutput);

	// output to the timers
	uint16_t posoutput = foutput > 0 ? roundf(foutput * hmotor->posTimer->Instance->ARR) : 0;
	uint16_t negoutput = foutput < 0 ? roundf(-foutput * hmotor->negTimer->Instance->ARR) : 0;

	*(&hmotor->posTimer->Instance->CCR1 + hmotor->pos_channel) = posoutput;
	if (hmotor->negTimer)
		*(&hmotor->negTimer->Instance->CCR1 + hmotor->neg_channel) = negoutput;
}
