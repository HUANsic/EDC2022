/*
 * huansic_types.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_TYPES_H_
#define INC_HUANSIC_TYPES_H_

typedef struct {
	uint8_t x, y;
}Coordinate;

typedef struct {
	Coordinate coord;
	uint8_t isAlly;
} Beacon;

typedef struct {
	Coordinate startCoord, destCoord;
	uint32_t timeLimit;
	uint8_t reward;
	uint32_t startTime;
} Order;

typedef struct{
	Coordinate coord1, coord2;
} Obstacle;

#endif /* INC_HUANSIC_TYPES_H_ */
