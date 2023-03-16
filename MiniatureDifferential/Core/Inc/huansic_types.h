/*
 * huansic_types.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_TYPES_H_
#define INC_HUANSIC_TYPES_H_

#include "stm32f1xx_hal.h"

typedef struct {
	int16_t x, y;		// 2 + 2
} Coordinate;			// = 4

typedef struct {
	Coordinate startCoord, destCoord;		// 4 + 4
	uint32_t timeLimit;						// 4
	uint32_t startTime;						// 4
	int32_t id;								// 4
	float reward;							// 4
} Order;									// = 24

/*
 typedef struct {
 Coordinate coord1, coord2;				// 4 + 4
 } Rectangle;								// = 8
 */
typedef struct {
	uint8_t x1, x2, y1, y2;
} Rectangle;
#endif /* INC_HUANSIC_TYPES_H_ */
