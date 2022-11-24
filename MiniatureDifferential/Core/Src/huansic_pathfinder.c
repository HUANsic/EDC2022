/*
 * huansic_pathfinder.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_pathfinder.h"

/* constant anchors */
const Coordinate outterCoords[8] = {
		{ .x = 35, .y = 35 },
		{ .x = 12714, .y = 35 },
		{ .x = 219, .y = 35 },
		{ .x = 219, .y = 127 },
		{ .x = 219, .y = 219 },
		{ .x = 127, .y = 219 },
		{ .x = 35, .y = 219 },
		{ .x = 35, .y = 127 }
};

const Coordinate innerCoords[4] = {
		{ .x = 127, .y = 43 },
		{ .x = 211, .y = 127 },
		{ .x = 127, .y = 211 },
		{ .x = 43, .y = 127 }
};

const Node outterNodes[8] = {
		{ .freeable = 0, .id = 0, .location = &outterCoords[0] },
		{ .freeable = 0, .id = 1, .location = &outterCoords[1] },
		{ .freeable = 0, .id = 2, .location = &outterCoords[2] },
		{ .freeable = 0, .id = 3, .location = &outterCoords[3] },
		{ .freeable = 0, .id = 4, .location = &outterCoords[4] },
		{ .freeable = 0, .id = 5, .location = &outterCoords[5] },
		{ .freeable = 0, .id = 6, .location = &outterCoords[6] },
		{ .freeable = 0, .id = 7, .location = &outterCoords[7] }
};

const Node innerNodes[4] = {
		{ .freeable = 0, .id = 12, .location = &innerCoords[0] },
		{ .freeable = 0, .id = 13, .location = &innerCoords[1] },
		{ .freeable = 0, .id = 14, .location = &innerCoords[2] },
		{ .freeable = 0, .id = 15, .location = &innerCoords[3] }
};

/* the map / connection matrix */
float outter_outter[8][8], outter_inner[8][4], outter_obstacle[8][20], inner_inner[4][4],
		inner_obstacle[4][20], obstacle_obstacle[20][20];
// 8x8+8x4+8x20+4x4+4x20+20x20=256+16+80+400=752
float start_all[32], dest_all[32];
float start_end;

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
