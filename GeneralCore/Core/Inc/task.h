/*
 * task.h
 *
 *  Created on: 2023年3月16日
 *      Author: Administrator
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

#include "main.h"
#include "positionpid.h"
#include "Astar_util.h"
#include "stm32f1xx_hal.h"
#include "huansic_types.h"

void Cal_Battery_Coord(void);

void Get_packet(Coordinate merchant);

void Send_packet(Coordinate consumer);

void go_Charge(void);
void set_Beacons(void);

Coordinate Get_nearest_consumer(void);
Coordinate Get_nearest_Beacon(void);

#endif /* INC_TASK_H_ */
