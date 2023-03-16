/*
 * positionpid.h
 *
 *  Created on: 2023年2月20日
 *      Author: ming
 */

#ifndef INC_POSITIONPID_H_
#define INC_POSITIONPID_H_

#include "huansic_types.h"
#include "ming_malloc.h"
#include "stdlib.h"
#include "math.h"
#include "collab_util.h"

uint8_t GotoDestination(Coordinate Destination, uint8_t mode);

float Angle_normalization(float angle);

float CalSpeed(int16_t x, int16_t y);

void Position_P(Coordinate *cur, Coordinate *goal);

uint8_t CheckCoord(void);

float Get_v_x(void);

float Get_v_y(void);

#endif /* INC_POSITIONPID_H_ */
