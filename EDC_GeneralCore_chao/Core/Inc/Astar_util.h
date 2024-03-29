/*
 * Astar_util.h
 *
 *  Created on: 2022年11月13日
 *      Author: Ming
 */

#ifndef INC_ASTAR_UTIL_H_
#define INC_ASTAR_UTIL_H_

#include "huansic_types.h"
#include "ming_types.h"

// free the entire A Star queue
void free_queue(A_Star_Node* head);
// append the Node into queue
void queue_append(A_Star_Node* head, A_Star_Node* NewNode);
// append the Node into list
void list_append(A_Star_Node* head, A_Star_Node* tail, A_Star_Node* NewNode);
// return the min Node which the Total is the min in the queue
A_Star_Node* TopNode(A_Star_Node* head, Map* map);
// calculate the H function
uint16_t cal_H(A_Star_Node* current, A_Star_Node* end);
// expand the queue
A_Star_Node* Find_around_node(A_Star_Node* current, A_Star_Node* head, Map* map, A_Star_Node* end, uint8_t step);
// edit the node which is in openlist
void Editcost(A_Star_Node* head, A_Star_Node* p);

// init_Map (No expand)
Map* init_map(Rectangle obstacles[5], Coordinate oppoBeacons[3]);
// init Node
A_Star_Node* init_Node(uint8_t x, uint8_t y, A_Star_Node* f, A_Star_Node* end, uint8_t step);

// A Star search
A_Star_Node* A_Star(Coordinate* start, Coordinate* end, uint8_t step);

#endif /* INC_ASTAR_UTIL_H_ */
