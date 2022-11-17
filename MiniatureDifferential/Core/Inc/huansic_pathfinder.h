/*
 * huansic_pathfinder.h
 *
 *  Created on: Nov 6, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_PATHFINDER_H_
#define INC_HUANSIC_PATHFINDER_H_

#include "huansic_types.h"

typedef struct Node_t{
	Coordinate *location;		// 4
	struct Node_t *lastNode;	// 4
	float distance;				// 4
	uint8_t processed;			// 1
	uint8_t id;					// 1
	uint8_t freeable;			// 1 (2)
} Node;

#endif /* INC_HUANSIC_PATHFINDER_H_ */
