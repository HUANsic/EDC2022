/*
 * collab_util.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "collab_util.h"

/* constants and definitions */
#define PATH_DEFAULT_LINEAR_SPEED 10	// in cm/s
#define PATH_DEFAULT_ANGULAR_SPEED 1	// in rad/s
#define MOTOR_TICKS_PER_REV 28		// per rotor turn
#define MOTOR_REDUCTION_RATIO 30
#define MOTOR_WHEEL_DIAMETER 33		// in mm

/* exported variables */
// game information 1
extern uint8_t gameStage;			// 0: pre-match(standby); 1: first half; 2: second half
extern uint8_t gameStatus;			// 0: standby; 1: running
extern uint32_t gameStageTimeLimit;		// in ms
extern uint32_t gameStageTimeSinceStart;	// in ms
extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate allyBeacons[3];		// ally charging station coordinate
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

// game information 2
extern Order *delivering[5];		// package picked up but not yet delivered

// game information 3
extern Coordinate myCoord;			// precise coordinate returned by game master
extern fCoordinate estimatedCoord;	// coordinate calculated by Kalman Filter
extern double angleZ, omegaZ, accelY;		// turning speed and linear acceleration
extern float myScore;				// current score returned by Master
extern float myCharge;				// current charge returned by Master

// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

Path* jymm_pathfind_straight(Coordinate *start, Coordinate *end){
	Path* StraightPath = huansic_path_new();
	StraightPath->start = *start;
	StraightPath->end = *end;
	StraightPath->type = linear;
	return StraightPath;
}

// add by ming-bot
// some A* utils
void init_map(Map* map) {
	// pass area
	for (int i = 0;i < 255; i++) {
		for (int j = 0;j < 255; j++) {
			map->a[i][j] = 0;
		}
	}
	// update abstacles
	for (int i = 0;i < 5;i ++) {
		Coordinate a1 = obstacles[i].coord1;
		Coordinate b2 = obstacles[i].coord2;
		// default Upper left corner and lower right corner
		for (int j = a1.x; j <= b2.x; j++) {
			for (int k = a1.y;k <= b2.y; k++) {
				map->a[j][k] = -1;
			}
		}
	}
	// update opponent charging station coordinate
	for (int i = 0; i < 3; i++) {
		map->a[oppoBeacons[i].x][oppoBeacons[i].y] = -1;
	}
}

void init_queue(Queue* L) {
	L->len = 0;
}

void copy_Node(Node* p1, Node* p2) {
	p1->x = p2->x;
	p1->y = p2->y;
	p1->F = p2->F;
	p1->C = p2->C;
	p1->H = p2->H;
	p1->father = p2->father;
}

void append(Queue* L, Node p) {
	L->len++;
	if (L->len >= MAXSIZE) {
		printf("ERROR!")
		return
	}
	newp = (Node*)malloc(sizeof(Node));
	for(int i = L->len - 2;i >= 0; i--) {
		if (L->data[i].F < p.F) {
			for (int j = i + 1; j < L->len;j ++) {
				copy_Node(newp, &(L->data[j]))
				copy_Node(&(L->data[j]), p)
				copy_Node(p, newp)
			}
		}
	}
}
void del(Queue* L, int index) {
	for (int i = index;i < L->len;i++ ) {
		copy_Node(&(L->data[i]),&(L->data[i + 1]));
 	}
	L->len--;
}
int getdex(Queue* L, node* p) {
	for (int i = 0;i < L->len;i++) {
  		if (L->data[i].x == p->x && L->data[i].y == p->y) return i;
	}
}
void print_Map(Map* G) {
	for (int i = 0;i < 255; i++) {
  		for (int j = 0;j < 255; j++)
   			printf("%d\t", G->a[i][j]);
  			printf("\n");
 	}
}

Node* startAstar(Coordinate start, Coordinate last) {
	startnode = (Node*)malloc(sizeof(Node));
	
}