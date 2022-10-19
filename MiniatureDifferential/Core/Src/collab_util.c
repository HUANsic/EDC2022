/*
 * collab_util.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "collab_util.h"

/* exported variables */
extern uint8_t gameStage, gameStatus;
extern Rectangle obstacles[5];
extern Coordinate allyBeacons[3];
extern Coordinate oppoBeacons[3];
extern Order *delivering[5];
extern Coordinate myCoord;
extern fCoordinate estimatedCoord;
extern double omegaZ, accelY;
extern uint32_t gameStageTimeLeft;
