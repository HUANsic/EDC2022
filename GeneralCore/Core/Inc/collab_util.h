/*
 * collab_util.h
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_COLLAB_UTIL_H_
#define INC_COLLAB_UTIL_H_

#include "huansic_types.h"

// evaluation; decide which path is better by analyzing the cost
float xiao_calculateCost(Path *path);

// find a path between two points (with lines and arcs)
Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end);
uint8_t mingyan_pathfind_avoidObstacle(Coordinate *start, Coordinate *end);

// convert Path to PWM/Duty Cycle/Motor Speed
void chao_move_angle(float _angle, float speed);

void move_angle_omega(float _angle, float speed);
// decide where to go
// void name_get_destination();

// Kalman filter
//void KalManInit(double x, double y);
//void KalManm_Predict(double X, double Y, double* _X, double* _Y);
void ming_coord_kalman_filter();

#endif /* INC_COLLAB_UTIL_H_ */
