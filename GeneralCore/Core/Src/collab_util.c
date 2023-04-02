/*
 * collab_util.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "collab_util.h"
#include "math.h"
#include "stdlib.h"

/* constants and definitions */
#define PATH_DEFAULT_LINEAR_SPEED 10	// in cm/s
#define PATH_DEFAULT_ANGULAR_SPEED 1	// in rad/s
#define PATH_MAX_TOLERANCE 3 	//in cm
#define PATH_THRESH_ANGLE M_PI / 2 // in rad
#define MOTOR_TICKS_PER_REV 28		// per rotor turn
#define MOTOR_REDUCTION_RATIO 30
#define MOTOR_WHEEL_DIAMETER 33		// in mm
#define MOTOR_REV_PER_CM 82 // in rev

/*inside functions*/
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define HALFLENGTH 14

/* exported variables */
// game information 1
extern uint8_t gameStage; // 0: pre-match(standby); 1: first half; 2: second half
extern uint8_t gameStatus;			// 0: standby; 1: running
extern uint32_t gameStageTimeLimit;		// in ms
extern uint32_t gameStageTimeSinceStart;	// in ms
extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate allyBeacons[3];		// ally charging station coordinate
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

// game information 2
extern Order *delivering[8];		// package picked up but not yet delivered

// game information 3
extern Coordinate myCoord;		// precise coordinate returned by game master
extern fCoordinate estimatedCoord;	// coordinate calculated by Kalman Filter
extern double angleZ, omegaZ, accelY;	// turning speed and linear acceleration
extern float myScore;				// current score returned by Master
extern float myCharge;				// current charge returned by Master
extern Motor_HandleTypeDef cmotor_lf, cmotor_rf, cmotor_lb, cmotor_rb;

// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

//0 - 360 degree, 0 degree front, clockwise
void chao_move_angle(float _angle, float speed) {
	float angle_arc = (_angle / 180) * M_PI;
	cmotor_lf.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
	cmotor_rf.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
	cmotor_lb.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
	cmotor_rb.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
}

void move_angle_omega(float _angle, float speed){
	float omega = _angle <= 180 ? _angle: (_angle - 360);
	if(abs(omega) < 15)
		omega = 0;
	else
		omega = 0.4 * omega;
	float angle_arc = (_angle / 180) * M_PI;
	if(omega * HALFLENGTH + 1.414 * speed > 3500)
		speed = 2000;
	cmotor_lf.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc) - omega * HALFLENGTH;
	cmotor_rf.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc) + omega * HALFLENGTH;
	cmotor_lb.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc) - omega * HALFLENGTH;
	cmotor_rb.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc) + omega * HALFLENGTH;
}

