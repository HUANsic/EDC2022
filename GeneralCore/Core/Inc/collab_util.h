/*
 * collab_util.h
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_COLLAB_UTIL_H_
#define INC_COLLAB_UTIL_H_

#include "huansic_types.h"

// convert Path to PWM/Duty Cycle/Motor Speed
void chao_move_angle(float _angle, float speed);

void move_angle_omega(float _angle, float speed);

#endif /* INC_COLLAB_UTIL_H_ */
