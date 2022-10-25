/*
 * collab_util.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "collab_util.h"

/* exported variables */
// game information 1
extern uint8_t gameStage;			// 0: pre-match(standby); 1: first half; 2: second half
extern uint8_t gameStatus;			// 0: standby; 1: running
extern uint32_t gameStageTimeLimit;		// in ms
extern uint32_t gameStageTimeSinceStart;	// in ms
extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate allyBeacons[3];		// ally charging station coordinate
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

// game information 2
extern Order *delivering[5];		// package picked up but not yet delivered

// game information 3
extern Coordinate myCoord;			// precise coordinate returned by game master
extern fCoordinate estimatedCoord;	// coordinate calculated by Kalman Filter
extern double angleZ, omegaZ, accelY;		// turning speed and linear acceleration
extern float myScore;				// current score returned by Master
extern float myCharge;				// current charge returned by Master

// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end){
	Path* StraightPath = huansic_path_new();
	StraightPath->start = *start;
	StraightPath->end = *end;
	StraightPath->type = linear;
	return StraightPath;
}
