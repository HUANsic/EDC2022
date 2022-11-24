#pragma once
/*
 * Astar_util.h
 *
 *  Created on: 2022Äê11ÔÂ13ÈÕ
 *      Author: Ming
 */
#include "huansic_types.h"
#include "ming_malloc.h"
#include "stdlib.h"
#include "stdio.h"
//#include "collab_util.h"

 // search if crash walls
uint8_t Find_crash(uint16_t cor, uint8_t flag);
// calculate the H function
uint16_t cal_H(uint16_t cor1, uint16_t cor2);
// return the direction info
int8_t dir(uint16_t cor1, uint16_t cor2);
// find the end
uint8_t findend(uint16_t cur, uint16_t end, uint8_t step);
// expand the queue
uint16_t Find_around_node(uint16_t currentindex, int8_t lastdir, A_Star_Node* end, uint8_t step);

// A Star search
void A_Star(Coordinate* start, Coordinate* last, uint8_t step);
