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

// decide where to go
// void name_get_destination();

// Kalman filter
// void ming_coord_kalman_filter();

//// add by ming-bot
//// some A* utils
//// init map (contain obstacles, beacon or something else)
//void init_map(Map* map);
//// init priority queue
//void init_queue(Queue* L);
//// copy Node
//void copy_Node(Node* p1, Node* p2);
//// append Node p in prority queue
//void append(Queue* L, Node p);
//// delet index
//void del(Queue* L, int index);
//int getdex(Queue* L, node* p);
//// print map for debug
//void print_Map(Map* G);
//// A* main body
//Node* startAstar(Coordinate start, Coordinate last);

#endif /* INC_COLLAB_UTIL_H_ */
