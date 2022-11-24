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
#define KA 1
#define KB 1
#define SPEED 3000
/*inside functions*/
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

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
extern Order *delivering[5];		// package picked up but not yet delivered

// game information 3
extern Coordinate myCoord;		// precise coordinate returned by game master
extern fCoordinate estimatedCoord;	// coordinate calculated by Kalman Filter
extern double angleZ, omegaZ, accelY;	// turning speed and linear acceleration
extern float myScore;				// current score returned by Master
extern float myCharge;				// current charge returned by Master
extern Motor_HandleTypeDef cmotor_lf, cmotor_rf, cmotor_lb, cmotor_rb;


// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end) {
	Path *straightPath = huansic_path_new();
	straightPath->start = *start;
	straightPath->end = *end;
	straightPath->type = linear;
	straightPath->speed = PATH_DEFAULT_LINEAR_SPEED;
	return straightPath;
}

void chao_move(Path* path){

	float x1 = path->start.x;
	float y1 = path->start.y;

	float x2 = path->end.x;
	float y2 = path->end.y;

	float l = sqrt(pow(myCoord.x - path->end.x, 2) + pow(myCoord.y - path->end.y, 2));
	float cos_t = (y2 - y1) / l;
	float sin_t = (x2 - x1) / l;

	float temp_lf = SPEED * cos_t - SPEED * sin_t;
	float temp_rf = SPEED * cos_t + SPEED * sin_t;
	float temp_lb = SPEED * cos_t + SPEED * sin_t;
	float temp_rb = SPEED * cos_t - SPEED * sin_t;

	while(sqrt(pow(myCoord.x - path->end.x, 2) + pow(myCoord.y - path->end.y, 2)) > 5){
		float d = (fabs((y2 - y1) * myCoord.x +(x1 - x2) * myCoord.y + ((x2 * y1) -(x1 * y2)))) / (sqrt(pow(y2 - y1, 2) + pow(x1 - x2, 2)));

	}
}

//0 - 360 degree, 0 degree front, clockwise
void chao_move_angle(float _angle, float speed){
	float angle_arc = (_angle / 180) * M_PI;
	cmotor_lf.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
	cmotor_rf.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
	cmotor_lb.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
	cmotor_rb.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
}
