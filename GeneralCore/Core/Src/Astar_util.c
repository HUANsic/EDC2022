/*
 * Astar_util.c
 *
 *  Created on: 2022年11月13日
 *      Author: Ming
 */

#include "Astar_util.h"

extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate
extern Queue openlist;
extern List closelist;

uint8_t Find_crash(uint16_t cor, uint8_t flag) {
	uint16_t x = cor / 256;
	uint16_t y = cor % 256;

	if (flag == 0) {
		return 1;
	}
	if (flag == 1) {
		//walls
		if ((x >= 38 && x <= 40) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))) {
			return 0;
		}
		if ((x >= 214 && x <= 216) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))) {
			return 0;
		}
		if ((y >= 38 && y <= 40) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))) {
			return 0;
		}
		if ((y >= 214 && y <= 216) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))) {
			return 0;
		}
		return 1;
	}
	//walls
	if ((x >= 38 && x <= 40) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))) {
		return 0;
	}
	if ((x >= 214 && x <= 216) && ((y >= 38 && y <= 107) || (y >= 147 && y <= 216))) {
		return 0;
	}
	if ((y >= 38 && y <= 40) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))) {
		return 0;
	}
	if ((y >= 214 && y <= 216) && ((x >= 38 && x <= 107) || (x >= 147 && x <= 216))) {
		return 0;
	}
	//obstacles
	for (uint8_t k = 0; k < 5; k++) {
		if (x >= obstacles[k].coord1.x && y >= obstacles[k].coord1.y && x <= obstacles[k].coord2.x
				&& y <= obstacles[k].coord2.y) {
			return 0;
		}
	}
	// oppoBeacons
	for (uint8_t k = 0; k <= 2; k++) {
		uint16_t distance = (x - oppoBeacons[k].x) * (x - oppoBeacons[k].x)
				+ (y - oppoBeacons[k].y) * (y - oppoBeacons[k].y);
		if (distance <= 400) {
			return 0;
		}
	}
	return 1;
}

uint16_t cal_H(uint16_t cor1, uint16_t cor2) {
	uint16_t x1 = cor1 / 256;
	uint16_t y1 = cor1 % 256;
	uint16_t x2 = cor2 / 256;
	uint16_t y2 = cor2 % 256;
	return abs(x1 - x2) + abs(y1 - y2);
}

int8_t dir(uint16_t cor1, uint16_t cor2) {
	uint16_t x1 = cor1 / 256;
	uint16_t y1 = cor1 % 256;
	uint16_t x2 = cor2 / 256;
	uint16_t y2 = cor2 % 256;
	return ((x2 - x1) + 2 * (y2 - y1));
}

uint8_t findend(uint16_t cur, uint16_t end, uint8_t step)
{
	uint16_t x1 = cur / 256;
	uint16_t y1 = cur % 256;
	uint16_t x2 = end / 256;
	uint16_t y2 = end % 256;
	if (abs(x2 - x1) < step && abs(y2 - y1) < step)
	{
		return 1;
	}
	else
		return 0;
}

void Find_around_node(uint16_t currentindex, int8_t lastdir, A_Star_Node *end, uint8_t step) {
	uint16_t x = openlist.buffer[currentindex].cor / 256;
	uint16_t y = openlist.buffer[currentindex].cor % 256;
	uint8_t obstacleflag = 1;		//负责检测采取何种程度的避障
	// left
	if (x >= step) {
		uint16_t curx = x - step;
		uint16_t cury = y;
		uint16_t coordinateindex = curx * 256 + cury;
		if (Find_crash(coordinateindex, obstacleflag)) {
			//not in close_list
			if (!findin_list(coordinateindex))
					{
				uint16_t index = findin_queue(coordinateindex);
				A_Star_Node newnode;
				newnode.cor = coordinateindex;
				newnode.fatherindex = currentindex;
				newnode.cost = openlist.buffer[currentindex].cost + step;
				//newnode.cost = openlist.buffer[currentindex].cost +
				newnode.total = newnode.cost + cal_H(coordinateindex, end->cor);
				if (dir(openlist.buffer[currentindex].cor, coordinateindex) != lastdir
						&& lastdir != 0)
					newnode.total += 10 * step;
				if (!index)	//not in open_list
				{
					queue_append(&newnode);
				}
				else	//in open_list
				{
					editcost(index, &newnode);
				}
			}
		}
	}
	// right
	if (x <= 255 - step) {
		uint16_t curx = x + step;
		uint16_t cury = y;
		uint16_t coordinateindex = curx * 256 + cury;
		if (Find_crash(coordinateindex, obstacleflag)) {
			//not in close_list
			if (!findin_list(coordinateindex))
					{
				uint16_t index = findin_queue(coordinateindex);
				A_Star_Node newnode;
				newnode.cor = coordinateindex;
				newnode.fatherindex = currentindex;
				newnode.cost = openlist.buffer[currentindex].cost + step;
				//newnode.cost = openlist.buffer[currentindex].cost +
				newnode.total = newnode.cost + cal_H(coordinateindex, end->cor);
				if (dir(openlist.buffer[currentindex].cor, coordinateindex) != lastdir
						&& lastdir != 0)
					newnode.total += 10 * step;
				if (!index)	//not in open_list
				{
					queue_append(&newnode);
				}
				else	//in open_list
				{
					editcost(index, &newnode);
				}
			}
		}
	}
	// up
	if (y >= step) {
		uint16_t curx = x;
		uint16_t cury = y - step;
		uint16_t coordinateindex = curx * 256 + cury;
		if (Find_crash(coordinateindex, obstacleflag)) {
			//not in close_list
			if (!findin_list(coordinateindex))
					{
				uint16_t index = findin_queue(coordinateindex);
				A_Star_Node newnode;
				newnode.cor = coordinateindex;
				newnode.fatherindex = currentindex;
				newnode.cost = openlist.buffer[currentindex].cost + step;
				//newnode.cost = openlist.buffer[currentindex].cost +
				newnode.total = newnode.cost + cal_H(coordinateindex, end->cor);
				if (dir(openlist.buffer[currentindex].cor, coordinateindex) != lastdir
						&& lastdir != 0)
					newnode.total += 10 * step;
				if (!index)	//not in open_list
				{
					queue_append(&newnode);
				}
				else	//in open_list
				{
					editcost(index, &newnode);
				}
			}
		}
	}
	// down
	if (y <= 255 - step) {
		uint16_t curx = x;
		uint16_t cury = y + step;
		uint16_t coordinateindex = curx * 256 + cury;
		if (Find_crash(coordinateindex, obstacleflag)) {
			//not in close_list
			if (!findin_list(coordinateindex))
					{
				uint16_t index = findin_queue(coordinateindex);
				A_Star_Node newnode;
				newnode.cor = coordinateindex;
				newnode.fatherindex = currentindex;
				newnode.cost = openlist.buffer[currentindex].cost + step;
				//newnode.cost = openlist.buffer[currentindex].cost +
				newnode.total = newnode.cost + cal_H(coordinateindex, end->cor);
				if (dir(openlist.buffer[currentindex].cor, coordinateindex) != lastdir
						&& lastdir != 0)
					newnode.total += 10 * step;
				if (!index)	//not in open_list
				{
					queue_append(&newnode);
				}
				else	//in open_list
				{
					editcost(index, &newnode);
				}
			}
		}
	}
}

uint8_t A_Star_main(Coordinate *start, Coordinate *last, uint8_t step) {
	A_Star_Node begin;
	A_Star_Node end;
	begin.cor = (start->x) * 256 + start->y;
	end.cor = (last->x) * 256 + last->y;
	begin.cost = 0;
	begin.total = 0;
	begin.fatherindex = 65535;
	queue_append(&begin);
	uint16_t curindex = 0;
	uint16_t Flag = 0;
	int8_t lastdir = 0;     		// left:-step up:-2*step right:step down:2*step
	while (1) {
		curindex = queue_top();
		if (curindex == 65535)
			return 0;
		if (openlist.buffer[curindex].fatherindex != 65535) {
			lastdir = dir(openlist.buffer[openlist.buffer[curindex].fatherindex].cor,
					openlist.buffer[curindex].cor);
		}
		list_append(openlist.buffer[curindex].cor);
		if(findend(openlist.buffer[curindex].cor, end.cor, step))
		{
			Flag = curindex;
			break;
		}
		Find_around_node(curindex, lastdir, &end, step);
	}

	//we now get the path
	Coordinate Astar_path[16];
	Astar_path[15].x = last->x;
	Astar_path[15].y = last->y;
	uint8_t i = 14;

	A_Star_Node *p = &openlist.buffer[Flag];
	if (p->cor != end.cor) {
		Astar_path[i].x = p->cor / 256;
		Astar_path[i].y = p->cor % 256;
		i--;
	}
	if (p->fatherindex != 65535)
		lastdir = dir(openlist.buffer[p->fatherindex].cor, p->cor);
	while (p->fatherindex != 65535) {
		if (lastdir != dir(openlist.buffer[p->fatherindex].cor, p->cor))
		{
			Astar_path[i].x = p->cor / 256;
			Astar_path[i].y = p->cor % 256;
			if (i == 1) {
				i = 0;
				p = &(openlist.buffer[p->fatherindex]);
				break;
			}
			i = i - 1;
			lastdir = dir(openlist.buffer[p->fatherindex].cor, p->cor);
		}
		p = &(openlist.buffer[p->fatherindex]);
	}
	Astar_path[i].x = p->cor / 256;
	Astar_path[i].y = p->cor % 256;

	Lane_init();
	uint8_t Laneis_OK = Insert_inLane(&Astar_path[0], i);

	return Laneis_OK;
}
