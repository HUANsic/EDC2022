/*
 * huansic_pathfinder.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_pathfinder.h"

/* constant anchors */
const Coordinate outterCoords[12] = {
		{ .x = 35, .y = 35 },
		{ .x = 110, .y = 35 },
		{ .x = 114, .y = 35 },
		{ .x = 219, .y = 35 },
		{ .x = 219, .y = 110 },
		{ .x = 219, .y = 144 },
		{ .x = 219, .y = 219 },
		{ .x = 144, .y = 219 },
		{ .x = 110, .y = 219 },
		{ .x = 35, .y = 219 },
		{ .x = 35, .y = 144 },
		{ .x = 35, .y = 110 }
};

const Coordinate innerCoords[8] = {
		{ .x = 110, .y = 43 },
		{ .x = 144, .y = 43 },
		{ .x = 211, .y = 110 },
		{ .x = 211, .y = 144 },
		{ .x = 144, .y = 211 },
		{ .x = 110, .y = 211 },
		{ .x = 43, .y = 144 },
		{ .x = 43, .y = 110 }
};

const Node outterNodes[12] = {
		{ .freeable = 0, .id = 0, .location = &outterCoords[0] },
		{ .freeable = 0, .id = 1, .location = &outterCoords[1] },
		{ .freeable = 0, .id = 2, .location = &outterCoords[2] },
		{ .freeable = 0, .id = 3, .location = &outterCoords[3] },
		{ .freeable = 0, .id = 4, .location = &outterCoords[4] },
		{ .freeable = 0, .id = 5, .location = &outterCoords[5] },
		{ .freeable = 0, .id = 6, .location = &outterCoords[6] },
		{ .freeable = 0, .id = 7, .location = &outterCoords[7] },
		{ .freeable = 0, .id = 8, .location = &outterCoords[8] },
		{ .freeable = 0, .id = 9, .location = &outterCoords[9] },
		{ .freeable = 0, .id = 10, .location = &outterCoords[10] },
		{ .freeable = 0, .id = 11, .location = &outterCoords[11] }
};

const Node innerNodes[8] = {
		{ .freeable = 0, .id = 12, .location = &innerCoords[0] },
		{ .freeable = 0, .id = 13, .location = &innerCoords[1] },
		{ .freeable = 0, .id = 14, .location = &innerCoords[2] },
		{ .freeable = 0, .id = 15, .location = &innerCoords[3] },
		{ .freeable = 0, .id = 16, .location = &innerCoords[4] },
		{ .freeable = 0, .id = 17, .location = &innerCoords[5] },
		{ .freeable = 0, .id = 18, .location = &innerCoords[6] },
		{ .freeable = 0, .id = 19, .location = &innerCoords[7] }
};

/* the map / connection matrix */
// naming convention: byte_bit[][]
uint16_t outter_outter[12];				// 12 x 12
uint8_t outter_inner[12];				// 12 x 8
uint8_t outter_obstacle[12][3];			// 12 x 20
uint8_t inner_inner[8];					// 8 x 8
uint8_t inner_obstacle[8][3];			// 8 x 20
uint8_t obstacle_obstacle[20][3];		// 20 x 20

/* external variables */
extern const Rectangle gameBoarder[8];

extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate allyBeacons[3];		// ally charging station coordinate
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

/* function prototypes */
uint8_t findCollision(Node *start, Node *end);

/* function bodies */
void updateMap() {
	// outter_outter

}

void initConnectionMap() {
	uint8_t i;
	// outter_outter
	outter_outter[0] = 0x0707;
	outter_outter[1] = 0x0B00;
	outter_outter[2] = 0x0D00;
	outter_outter[3] = 0x0EE0;
	outter_outter[4] = 0x0160;
	outter_outter[5] = 0x01A0;
	outter_outter[6] = 0x01DC;
	outter_outter[7] = 0x002C;
	outter_outter[8] = 0x0038;
	outter_outter[9] = 0x083B;
	outter_outter[10] = 0x0805;
	outter_outter[11] = 0x0806;

	// outter_inner
	for (i = 0; i < 12; i++)
		outter_inner[i] = 0x00;

	// outter_obstacle
	for (i = 0; i < 12; i++) {
		outter_obstacle[i][0] = 0x00;
		outter_obstacle[i][1] = 0x00;
		outter_obstacle[i][2] = 0x00;
	}

	// inner_inner
	for (i = 0; i < 8; i++)
		inner_inner[i] = 0x00;

	// inner_obstacle
	for (i = 0; i < 8; i++) {
		inner_obstacle[i][0] = 0x00;
		inner_obstacle[i][1] = 0x00;
		inner_obstacle[i][2] = 0x00;
	}

	// obstacle_obstacle
	for (i = 0; i < 8; i++) {
		obstacle_obstacle[i][0] = 0x00;
		obstacle_obstacle[i][1] = 0x00;
		obstacle_obstacle[i][2] = 0x00;
	}
}

uint8_t findCollision(Node *start, Node *end) {
	Rectangle *currentBox;
	float temp;
	uint8_t i;

	/* test boarders */
	for (i = 0; i < 8; i++) {
		currentBox = &gameBoarder[i];
		// left edge
		if ((start->location->x <= currentBox->coord1.x)
				&& (end->location->x >= currentBox->coord1.x)) {	// if x is in between
			// then check y
			temp = start->location->y + (float) (currentBox->coord1.x - start->location->x)
					* (end->location->y - start->location->y)
					/ (end->location->x - start->location->x);
			if ((currentBox->coord1.y <= temp) && (currentBox->coord2.y >= temp)) {
				return 2;
			}
		}
		// right edge
		if ((start->location->x <= currentBox->coord2.x)
				&& (end->location->x >= currentBox->coord2.x)) {	// if x is in between
			// then check y
			temp = start->location->y + (float) (currentBox->coord2.x - start->location->x)
					* (end->location->y - start->location->y)
					/ (end->location->x - start->location->x);
			if ((currentBox->coord1.y <= temp) && (currentBox->coord2.y >= temp)) {
				return 2;
			}
		}
		// top edge
		if ((start->location->y <= currentBox->coord1.y)
				&& (end->location->y >= currentBox->coord1.y)) {	// if y is in between
			// then check x
			temp = start->location->x + (float) (currentBox->coord1.y - start->location->y)
					* (end->location->x - start->location->x)
					/ (end->location->y - start->location->y);
			if ((currentBox->coord1.x <= temp) && (currentBox->coord2.x >= temp)) {
				return 2;
			}
		}
		// bottom edge
		if ((start->location->y <= currentBox->coord2.y)
				&& (end->location->y >= currentBox->coord2.y)) {	// if y is in between
			// then check x
			temp = start->location->x + (float) (currentBox->coord2.y - start->location->y)
					* (end->location->x - start->location->x)
					/ (end->location->y - start->location->y);
			if ((currentBox->coord1.x <= temp) && (currentBox->coord2.x >= temp)) {
				return 2;
			}
		}
	}

	/* test obstacles */
	for (i = 0; i < 5; i++) {
		currentBox = &obstacles[i];
		// left edge
		if ((start->location->x <= currentBox->coord1.x)
				&& (end->location->x >= currentBox->coord1.x)) {	// if x is in between
			// then check y
			temp = start->location->y + (float) (currentBox->coord1.x - start->location->x)
					* (end->location->y - start->location->y)
					/ (end->location->x - start->location->x);
			if ((currentBox->coord1.y <= temp) && (currentBox->coord2.y >= temp)) {
				return 1;
			}
		}
		// right edge
		if ((start->location->x <= currentBox->coord2.x)
				&& (end->location->x >= currentBox->coord2.x)) {	// if x is in between
			// then check y
			temp = start->location->y + (float) (currentBox->coord2.x - start->location->x)
					* (end->location->y - start->location->y)
					/ (end->location->x - start->location->x);
			if ((currentBox->coord1.y <= temp) && (currentBox->coord2.y >= temp)) {
				return 1;
			}
		}
		// top edge
		if ((start->location->y <= currentBox->coord1.y)
				&& (end->location->y >= currentBox->coord1.y)) {	// if y is in between
			// then check x
			temp = start->location->x + (float) (currentBox->coord1.y - start->location->y)
					* (end->location->x - start->location->x)
					/ (end->location->y - start->location->y);
			if ((currentBox->coord1.x <= temp) && (currentBox->coord2.x >= temp)) {
				return 1;
			}
		}
		// bottom edge
		if ((start->location->y <= currentBox->coord2.y)
				&& (end->location->y >= currentBox->coord2.y)) {	// if y is in between
			// then check x
			temp = start->location->x + (float) (currentBox->coord2.y - start->location->y)
					* (end->location->x - start->location->x)
					/ (end->location->y - start->location->y);
			if ((currentBox->coord1.x <= temp) && (currentBox->coord2.x >= temp)) {
				return 1;
			}
		}
	}

	// if no collision at all, return 0
	return 0;
}

uint8_t tryEdge() {

}
