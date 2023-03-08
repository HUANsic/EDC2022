/*
 * ming_malloc.c
 *
 *  Created on: 2022年11月21日
 *      Author: Ming
 */
#include "ming_malloc.h"

Queue openlist;
List closelist;

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
		if (openlist.buffer[i + openlist.Head].cor == c) return i + openlist.Head;
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
		openlist.buffer[index] = *node;
}
