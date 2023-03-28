/*
 * ming_malloc.c
 *
 *  Created on: 2022年11月21日
 *      Author: Ming
 */
#include "ming_malloc.h"

Queue openlist;
List closelist;
Lane pathlane;
Order_list orders;
extern Coordinate myCoord;
extern Coordinate exitpoints[8];

void Queue_init(void)
{
	openlist.Head = 0;
	openlist.Length = 0;
}

void List_init(void)
{
	closelist.Head = 0;
	closelist.Tail = 0;
	closelist.Length = 0;
}

void Lane_init(void){
	pathlane.Head = 0;
	pathlane.Length = 0;
}

uint8_t queue_append(A_Star_Node* node)
{
	if (openlist.Length + openlist.Head >= QUEUE_SIZE)
	{
		return 0;
	}
	else
	{
		uint16_t i = 0;
		while (i < openlist.Length)
		{
			if (openlist.buffer[i + openlist.Head].total <= node->total)
			{
				i++;
			}
			else
			{
				A_Star_Node change;
				while (i < openlist.Length + 1)
				{
					change = openlist.buffer[i + openlist.Head];
					openlist.buffer[i + openlist.Head] = *node;
					*node = change;
					i++;
				}
				openlist.Length += 1;
				return 1;
			}
		}
		openlist.buffer[openlist.Head + openlist.Length] = *node;
		openlist.Length++;
		return 1;
	}
}

uint8_t list_append(uint16_t c)
{
	if (closelist.Length >= LIST_SIZE)
	{
		return 0;
	}
	else
	{
		closelist.buffer[closelist.Tail] = c;
		closelist.Tail = (closelist.Tail + 1) % LIST_SIZE;
		closelist.Length++;
		return 1;
	}
}

uint16_t queue_top()
{
	if (openlist.Length > 0)
	{
		openlist.Head++;
		openlist.Length--;
		return openlist.Head - 1;
	}
	else {
		return 65535;
	}
}

uint16_t findin_queue(uint16_t c)
{
	uint16_t i;
	for (i = 0; i < openlist.Length; i++)
	{
		if (openlist.buffer[i + openlist.Head].cor == c) return (i + openlist.Head);
	}
	return 0;
}

uint16_t findendin_queue(uint16_t c)
{
	uint16_t i;
	uint16_t x1 = c / 256;
	uint16_t y1 = c % 256;
	for (i = 0; i < openlist.Length; i++)
	{
		uint16_t x = openlist.buffer[i + openlist.Head].cor / 256;
		uint16_t y = openlist.buffer[i + openlist.Head].cor % 256;
		if (abs(x - x1) < 5 && abs(y - y1) < 5) return (i + openlist.Head);
	}
	return 0;
}

uint8_t findin_list(uint16_t c)
{
	uint16_t i;
	for (i = 0; i < closelist.Length; i++)
	{
		if (closelist.buffer[i + closelist.Head] == c) return 1;
	}
	return 0;
}

void editcost(uint16_t index, A_Star_Node* node)
{
	if (openlist.buffer[index].total > node->total)
	{
		openlist.buffer[index].total = node->total;
		openlist.buffer[index].cost = node->cost;
		openlist.buffer[index].fatherindex = node->fatherindex;
	}
}

uint8_t Insert_inLane(Coordinate *head_coor, uint8_t head_index)
{
	if(!head_coor) return 0;
	pathlane.Head = 0;
	pathlane.Length = 16 - head_index;
	for(uint8_t i = head_index; i < 16; i++)
	{
		pathlane.buffer[pathlane.Head + i - head_index] = *(head_coor + i);
	}
	return 1;
}

void order_list_init(void)
{
	orders.length = 0;
	orders.new = 0;
}

void order_append(Order *an_order)
{
	if((an_order->startCoord.x != orders.buffer[orders.new].x) || (an_order->startCoord.y != orders.buffer[orders.new].y))
	{
		orders.buffer[orders.length].x = an_order->startCoord.x;
		orders.buffer[orders.length].y = an_order->startCoord.y;
		orders.new = orders.length;
		orders.length = orders.length + 1;
	}
}

int8_t Get_nearest_order(void)
{
	uint8_t i;
	int16_t mindis = 512;
	int8_t minindex = -1;
	if(orders.length == 0)
		return minindex;
	for(i=0;i < orders.length; i++)
	{
		int16_t distance = abs(orders.buffer[i].x - myCoord.x) + abs(orders.buffer[i].y - myCoord.y);
		if(distance < mindis)
		{
			mindis = distance;
			minindex = i;
		}
	}
    return minindex;
}

void exitpoints_init(void){
	exitpoints[0].x = 127;
	exitpoints[0].y = 18;
	exitpoints[1].x = 127;
	exitpoints[1].y = 60;
	exitpoints[2].x = 236;
	exitpoints[2].y = 127;
	exitpoints[3].x = 194;
	exitpoints[3].y = 127;
	exitpoints[4].x = 127;
	exitpoints[4].y = 236;
	exitpoints[5].x = 127;
	exitpoints[5].y = 194;
	exitpoints[6].x = 18;
	exitpoints[6].y = 127;
	exitpoints[7].x = 60;
	exitpoints[7].y = 127;
}
