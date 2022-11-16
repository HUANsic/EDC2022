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

void queue_append(A_Star_Node** head, A_Star_Node* NewNode) {
	if(*head == NULL) {
		*head = NewNode;
	}
	else
	{
		if ((*head)->Total > NewNode->Total) {
			NewNode->next = (*head);
			(*head) = NewNode;
		}
		else {
			A_Star_Node* p = (*head);
			uint8_t isappend = 0;
			while (p->next != NULL) {
				if (p->next->Total > NewNode->Total) {
					NewNode->next = p->next;
					p->next = NewNode;
					isappend = 1;
					break;
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
	}
	NewNode = NULL;
}

void list_append(A_Star_Node** head, A_Star_Node** tail, A_Star_Node* NewNode){
	if((*head) == NULL){
		(*head) = NewNode;
		(*tail) = NewNode;
	}
	else{
		(*tail)->next = NewNode;
		(*tail) = NewNode;
	}
	NewNode = NULL;
}

uint8_t Find_ifin(uint16_t x, uint16_t y, A_Star_Node* head) {
	A_Star_Node* p = head;
	while (p != NULL) {
		if(p->x == x && p->y == y) {
			return 0;
		}
		else {
			p = p->next;
		}
	}
	return 1;
}

uint8_t Find_crash(uint16_t x, uint16_t y) {
	//walls
	if((x >= 38 && x <= 40) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))) {
		return 0;
	}
	if((x >= 214 && x <= 216) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))){
		return 0;
	}
	if((y >= 38 && y <= 40) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))){
		return 0;
	}
	if((y >= 214 && y <= 216) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))){
		return 0;
	}
	//obstacles
	for(uint8_t k = 0;k < 5; k++){
		if(x >= obstacles[k].coord1.x && y >= obstacles[k].coord1.y && x <= obstacles[k].coord2.x && y <= obstacles[k].coord2.y){
			return 0;
		}
	}
	// oppoBeacons
	for(uint8_t k = 0;k <= 2; k++){
		uint16_t distance = (x - oppoBeacons[k].x)*(x - oppoBeacons[k].x)+(y - oppoBeacons[k].y)*(y - oppoBeacons[k].y);
		if(distance <= 400){
			return 0;
		}
	}
	return 1;
}

A_Star_Node* TopNode(A_Star_Node** head, A_Star_Node** closehead, A_Star_Node** closetail) {
	A_Star_Node* p = (*head);
	(*head) = p->next;
	p->next = NULL;
	list_append(&closehead, &closetail, p);
	return p;
}

uint16_t cal_H(A_Star_Node* current, A_Star_Node* end) {
	return abs(current->x - end->x) + abs(current->y - end->y);
}

void Editcost(A_Star_Node** head, A_Star_Node* p) {
	A_Star_Node* t = (*head);
	if(((*head)->x == p->x) && ((*head)->y == p->y)) {
		if(p->Cost < (*head)->Cost) {
			(*head) = (*head)->next;
			free(t);
		}
	}
	else {
		A_Star_Node* l = NULL;
		while (t->next != NULL) {
			if ((t->next->x == p->x) && (t->next->y == p->y)) {
				if (p->Cost < t->next->Cost) {
					l = t->next;
					t->next = l->next;
					free(l);
					l = NULL;
				}
				break;
			}
			else
			{
				t = t->next;
			}
		}
	}
	t = NULL;
	queue_append(head, p);
}

int8_t dir(A_Star_Node* from, A_Star_Node* to){
	return ((to->x - from->x) + 2*(to->y - from->y));
}

A_Star_Node* init_Node(uint16_t x, uint16_t y, A_Star_Node* f, int8_t lastdir, A_Star_Node* end, uint8_t step) {
	A_Star_Node* p = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	p->x = x;
	p->y = y;
	p->father = f;
	p->next = NULL;
	p->Cost = f->Cost + abs(step);
	if(lastdir != 0) {
		if(lastdir != dir(f, p)){
			p->Cost = p->Cost + 10*step;
		}
	}
	p->Total = p->Cost + cal_H(p, end);
	return p;
}

A_Star_Node* Find_around_node(A_Star_Node* current, int8_t lastdir, A_Star_Node** openhead, A_Star_Node* closehead, A_Star_Node* end, uint8_t step) {
	// left
	if(current->x >= step) {
		uint16_t curx = current->x - step;
		uint16_t cury = current->y;
		if (Find_crash(curx, cury)) {
			if (openhead != NULL) {
				if (Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
				else {
					if (!Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
						A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
						Editcost(openhead, p);
					}
				}
			}
			else {
				if (Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
			}
		}
	}
	// right
	if(current->x <= 255 - step) {
		uint16_t curx = current->x + step;
		uint16_t cury = current->y;
		if (Find_crash(curx, cury)) {
			if (openhead != NULL) {
				if (Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
				else {
					if (!Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
						A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
						Editcost(openhead, p);
					}
				}
			}
			else {
				if (Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
			}
		}
	}
	// up
	if(current->y >= step) {
		uint16_t curx = current->x;
		uint16_t cury = current->y - step;
		if (Find_crash(curx, cury)) {
			if (openhead != NULL) {
				if (Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
				else {
					if (!Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
						A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
						Editcost(openhead, p);
					}
				}
			}
			else {
				if (Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
			}
		}
	}
	// down
	if(current->y <= 255 - step) {
		uint16_t curx = current->x;
		uint16_t cury = current->y + step;
		if (Find_crash(curx, cury)) {
			if (openhead != NULL) {
				if (Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
				else {
					if (!Find_ifin(curx, cury, (*openhead)) && Find_ifin(curx, cury, closehead)) {
						A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
						Editcost(openhead, p);
					}
				}
			}
			else {
				if (Find_ifin(curx, cury, closehead)) {
					A_Star_Node* p = init_Node(curx, cury, current, lastdir, end, step);
					if (p->Cost == p->Total) return p;
					queue_append(openhead, p);
				}
			}
		}
	}
	return NULL;
}

A_Star_Node* A_Star(Coordinate* start, Coordinate* end, uint8_t step) {
	A_Star_Node* openhead = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	A_Star_Node* closehead = NULL;
	A_Star_Node* closetail = NULL;

	openhead->x = start->x;
	openhead->y = start->y;
	openhead->father = NULL;
	openhead->next = NULL;
	openhead->Cost = 0;
	openhead->Total = abs(start->x - end->x) + abs(start->y - end->y);

	A_Star_Node* END = (A_Star_Node*)malloc(sizeof(A_Star_Node));
	END->x = end->x;
	END->y = end->y;
	END->father = NULL;

	A_Star_Node* CurNode = NULL;
	A_Star_Node* Flag = NULL;
	int8_t lastdir = 0;     		// left:-step up:-2*step right:step down:2*step
	while(1){
		CurNode = TopNode(&openhead, &closehead, &closetail);
		if(CurNode->father != NULL) {
			lastdir = dir(CurNode->father, CurNode);
		}
		list_append(&closehead, &closetail, CurNode);
		Flag = Find_around_node(CurNode, lastdir, &openhead, closehead, END, step);
		if (Flag != NULL) {
			free_queue(openhead);
			openhead = NULL;
			free(CurNode);
			free(END);
			END = NULL;
			// closelist is not freed
			break;
		}
	}

//	A_Star_Node* p = Flag;
//	while (p != NULL) {
//		printf("\n(x,y):{%d;%d}", p->x, p->y);
//		p = p->father;
//	}
//	return Flag;
	 //we now get the path
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
	 op = NULL;
	 straightPath = NULL;

	 return huanpathhead;
}



