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
Path* shang_pathfind_ignoreObstacle_verticle(Coordinate *start, Coordinate *end);
Path* shang_pathfind_ignoreObstacle_horizontal(Coordinate *start, Coordinate *end);
Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end);
Path* mingyan_pathfind_avoidObstacle(Coordinate *start, Coordinate *end);

// convert Path to PWM/Duty Cycle/Motor Speed
void chao_move(Path *path);

void chao_move_angle(float _angle, float speed);

// decide where to go
// void name_get_destination();

// Kalman filter
// void ming_coord_kalman_filter();

#endif /* INC_COLLAB_UTIL_H_ */
