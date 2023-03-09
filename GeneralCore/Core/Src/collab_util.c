/*
 * collab_util.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "collab_util.h"
#include "Astar_util.h"
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
extern Motor_HandleTypeDef hmotor_left, hmotor_right;
extern Motor_HandleTypeDef cmotor_lf, cmotor_rf, cmotor_lb, cmotor_rb;

// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end) {
	Path *straightPath = huansic_path_new();
	if (!straightPath)
		return straightPath;
	straightPath->start = *start;
	straightPath->end = *end;
	return straightPath;
}

void chao_move(Path *path) {
	float currentAngle;
	float diffAngle;
	int8_t isClockwise;

	uint8_t targetX = path->end.x;
	uint8_t targetY = path->end.y;

	uint8_t currentX = myCoord.x;
	uint8_t currentY = myCoord.y;

	while (!(abs(currentX - targetX) < PATH_MAX_TOLERANCE
			&& abs(currentY - targetY) < PATH_MAX_TOLERANCE)) {

		if (currentX == targetX) {
			if (currentY < targetY) {
				currentAngle = M_PI / 2;
			} else {
				currentAngle = -M_PI / 2;
			}
		} else if (targetY >= currentY) {
			if (targetX > currentX) {
				currentAngle = atan((float) (targetY - currentY) / (targetX - currentX));
			} else {
				currentAngle = M_PI + atan((float) (targetY - currentY) / (targetX - currentX));
			}
		} else {
			if (targetX > currentX) {
				currentAngle = atan((float) (targetY - currentY) / (targetX - currentX));
			} else {
				currentAngle = -M_PI
						+ atan((float) (targetY - currentY) / (targetX - currentX));
			}
		}

		if (max(currentAngle, angleZ) - min(currentAngle, angleZ)
				> 2 * M_PI - (max(currentAngle, angleZ) - min(currentAngle, angleZ))) {
			diffAngle = 2 * M_PI - max(currentAngle, angleZ) + min(currentAngle, angleZ);
			if (currentAngle > angleZ) {
				isClockwise = 1;
			} else {
				isClockwise = -1;
			}
		} else {
			diffAngle = max(currentAngle, angleZ) - min(currentAngle, angleZ);
			if (currentAngle > angleZ) {
				isClockwise = -1;
			} else {
				isClockwise = 1;
			}
		}
		if (diffAngle < PATH_THRESH_ANGLE) {
			uint8_t k = 10;
			hmotor_left.goalSpeed = PATH_DEFAULT_LINEAR_SPEED
					* MOTOR_REV_PER_CM + isClockwise * k * diffAngle;
			hmotor_right.goalSpeed = PATH_DEFAULT_LINEAR_SPEED
					* MOTOR_REV_PER_CM - isClockwise * k * diffAngle;
		} else {
			uint8_t k = 10;
			hmotor_left.goalSpeed = isClockwise * k * diffAngle;
			hmotor_right.goalSpeed = isClockwise * k * diffAngle;
		}
	}
}

//0 - 360 degree, 0 degree front, clockwise
void chao_move_angle(float _angle, float speed) {
	float angle_arc = (_angle / 180) * M_PI;
	cmotor_lf.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
	cmotor_rf.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
	cmotor_lb.goalSpeed = speed * cos(angle_arc) + speed * sin(angle_arc);
	cmotor_rb.goalSpeed = speed * cos(angle_arc) - speed * sin(angle_arc);
}

Path* mingyan_pathfind_avoidObstacle(Coordinate *start, Coordinate *end) {
	return A_Star_main(start, end, 5);
}

