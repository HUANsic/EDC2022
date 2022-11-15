/*
 * Astar_util.c
 *
 *  Created on: 2022年11月13日
 *      Author: Ming
 */

#include "Astar_util.h"
#include "stdlib.h"
#include "collab_util.h"

extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

void free_queue(A_Star_Node* head) {
	A_Star_Node* p = head;
	while (p != NULL) {
		head = p->next;
		free(p);
		p = head;
	}
}

void queue_append(A_Star_Node* head, A_Star_Node* NewNode) {
	if(head == NULL) {
		head = NewNode;
	}
	if( head->Total > NewNode->Total) {
		NewNode->next = head;
		head = NewNode;
	}
	else {
		A_Star_Node* p = head;
		uint8_t isappend = 0;
		while (p->next != NULL) {
			if ( p->next->Total > NewNode->Total) {
				NewNode->next = p->next;
				p->next = NewNode;
				isappend = 1;
			}
			else {
				p = p->next;
			}
		}
		if (isappend == 0) {
			p->next = NewNode;
		}
		p = NULL;
	}
	NewNode = NULL;
}
void list_append(A_Star_Node* head, A_Star_Node* tail, A_Star_Node* NewNode){
	if(head == NULL){
		head = NewNode;
		tail = NewNode;
	}
	else{
		tail->next = NewNode;
		tail = NewNode;
	}
	NewNode = NULL;
}

A_Star_Node* TopNode(A_Star_Node* head, Map* map) {
	A_Star_Node* p = head;
	head = p->next;
	p->next = NULL;
	map->map_icon[p->x][p->y] = 2;
	return p;
}

uint16_t cal_H(A_Star_Node* current, A_Star_Node* end) {
	return abs(current->x - end->x) + abs(current->y - end->y);
}

void Editcost(A_Star_Node* head, A_Star_Node* p) {
	A_Star_Node* t = head;
	if((head->x == p->x) && (head->y == p->y)) {
		if(p->Cost < head->Cost) {
			head = head->next;
			free(t);
		}
	}
	while(t->next != NULL) {
		if((t->next->x == p->x) && (t->next->y == p->y)) {
			if(p->Cost < t->next->Cost) {
				A_Star_Node* l = t->next;
				t->next = l->next;
				free(l);
				l = NULL;
			}
		break;
		}
	}
	t = NULL;
	queue_append(head, p);
}
int8_t dir(A_Star_Node* from, A_Star_Node* to){
	return ((to->x - from->x) + 2*(to->y - from->y));
}

A_Star_Node* init_Node(uint8_t x, uint8_t y, A_Star_Node* f, int8_t lastdir, A_Star_Node* end, uint8_t step) {
	A_Star_Node* p = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	p->x = x;
	p->y = y;
	p->father = f;
	p->next = NULL;
	p->Cost = f->Cost + abs(step);
	if(lastdir != 0) {
		if(lastdir != dir(f, p)){
			p->Cost = p->Cost + 10;
		}
	}
	p->Total = p->Cost + cal_H(p, end);
	return p;
}

A_Star_Node* Find_around_node(A_Star_Node* current, int8_t lastdir, A_Star_Node* head, Map* map, A_Star_Node* end, uint8_t step) {
	// left
	if(current->x >= step) {
		uint8_t curx = current->x - step;
		uint8_t cury = current->y;
		if(map->map_icon[curx][cury] == 0) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			if(p->Cost == p->Total) return p;
			map->map_icon[curx][cury] = 1;
			queue_append(head, p);
		}
		if(map->map_icon[curx][cury] == 1) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			Editcost(head, p);
		}
	}
	// right
	if(current->x <= 255 - step) {
		uint8_t curx = current->x + step;
		uint8_t cury = current->y;
		if(map->map_icon[curx][cury] == 0) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			if(p->Cost == p->Total) return p;
			map->map_icon[curx][cury] = 1;
			queue_append(head, p);
		}
		if(map->map_icon[curx][cury] == 1) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			Editcost(head, p);
		}
	}
	// up
	if(current->y >= step) {
		uint8_t curx = current->x;
		uint8_t cury = current->y - step;
		if(map->map_icon[curx][cury] == 0) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			if(p->Cost == p->Total) return p;
			map->map_icon[curx][cury] = 1;
			queue_append(head, p);
		}
		if(map->map_icon[curx][cury] == 1) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			Editcost(head, p);
		}
	}
	// down
	if(current->y <= 255 - step) {
		uint8_t curx = current->x;
		uint8_t cury = current->y + step;
		if(map->map_icon[curx][cury] == 0) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			if(p->Cost == p->Total) return p;
			map->map_icon[curx][cury] = 1;
			queue_append(head, p);
		}
		if(map->map_icon[curx][cury] == 1) {
			A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
			Editcost(head, p);
		}
	}
	return NULL;
}

Map* init_map(Rectangle obstacles[5], Coordinate oppoBeacons[3]) {
	Map* map = (Map*)malloc(sizeof(Map));
	for(uint8_t i = 0;i <= 255; i++) {
		for(uint8_t j = 0;j <= 255; j++) {
			//walls
			if((i >= 38 && i <= 40) && ((j >= 38 && j <= 107) || (j >= 147 && j <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((i >= 214 && i <= 216) && ((j >= 38 && j <= 107) || (j >= 147 && j <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((j >= 38 && j <= 40) && ((i >= 38 && i <= 107) || (i >= 147 && i <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			if((j >= 214 && j <= 216) && ((i >= 38 && i <= 107) || (i >= 147 && i <= 216))){
				map->map_icon[i][j] = 3;
				continue;
			}
			// obstacles
			for(uint8_t k = 0;k <= 4; k++){
				if(i >= obstacles[k].coord1.x && j >= obstacles[k].coord1.y && i <= obstacles[k].coord2.x && j <= obstacles[k].coord2.y){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
			// oppoBeacons
			for(uint8_t k = 0;k <= 2; k++){
				uint16_t distance = (i - oppoBeacons[k].x)*(i - oppoBeacons[k].x)+(j - oppoBeacons[k].y)*(j - oppoBeacons[k].y);
				if(distance <= 400){
					map->map_icon[i][j] = 3;
					continue;
				}
			}
		}
	}
	return map;
}


A_Star_Node* A_Star(Coordinate* start, Coordinate* end, uint8_t step) {
	A_Star_Node* head = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	A_Star_Node* closehead = NULL;
	A_Star_Node* closetail = NULL;

	head->x = start->x;
	head->y = start->y;
	head->father = NULL;
	head->next = NULL;
	head->Cost = 0;
	head->Total = abs(start->x - end->x) + abs(start->y - end->y);

	A_Star_Node* END = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	END->x = end->x;
	END->y = end->y;
	END->father = NULL;
	head->next = NULL;

	Map* map = init_map(obstacles, oppoBeacons);
	A_Star_Node* CurNode = NULL;
	A_Star_Node* Flag = NULL;
	int8_t lastdir = 0;     // left:-step up:-2*step right:step down:2*step
	while(1){
		CurNode = TopNode(head, map);
		if(CurNode->father != NULL) {
			lastdir = dir(CurNode->father, CurNode);
		}
		list_append(closehead, closetail, CurNode);
		Flag = Find_around_node(CurNode, lastdir, head, map, END, step);
		if (Flag != NULL) {
			free_queue(head);
			free(map);
			// closelist is not freed
			break;
		}
	}
	// we now get the path
	Coordinate Astar_path[8];
	Astar_path[7].x = Flag->x;
	Astar_path[7].y = Flag->y;
	uint8_t i = 6;
	A_Star_Node* m = Flag;
	lastdir = dir(m->father, m);
	while(m->father != NULL){
		if(lastdir != dir(m->father, m)) {
			Astar_path[i].x = m->father->x;
			Astar_path[i].y = m->father->y;
			if(i == 1){
				i = 0;
				break; //Fail
			}
			i = i - 1;
			lastdir = dir(m->father, m);
		}
		m = m->father;
	}
	Astar_path[i].x = m->x;
	Astar_path[i].y = m->y;
	Path* huanpathhead = jymm_pathfind_straight(&Astar_path[i], &Astar_path[i+1]);
	Path* op = huanpathhead;
	Path* straightPath = NULL;
	for(i = i + 1;i < 7;i++){
		straightPath = jymm_pathfind_straight(&Astar_path[i], &Astar_path[i+1]);
		huansic_path_cascade(op, straightPath);
		op = straightPath;
	}
	free_queue(closehead);
	free(Flag);
	Flag = NULL;
	A_Star_Node* closehead = NULL;
	A_Star_Node* closetail = NULL;
	free(END);
	END = NULL;
	op = NULL;
	straightPath = NULL;

	return huanpathhead;
}




