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

void Cal_Battery_Coord();

void Get_packet();

void Send_packet();

void go_Charge();

#endif /* INC_TASK_H_ */
