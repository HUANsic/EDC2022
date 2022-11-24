/*
 * ming_malloc.h
 *
 *  Created on: 2022Äê11ÔÂ21ÈÕ
 *      Author: Ming
 */

#ifndef INC_MING_MALLOC_H_
#define INC_MING_MALLOC_H_

//#include "stm32f1xx_hal.h"
#include "stdint.h"


#define QUEUE_SIZE 256
#define LIST_SIZE 128


#pragma pack (2)
 // A* Node
typedef struct A_Star_Node_t {
	uint16_t cor;                   // 2
	float cost;                     // 4
	float total;                    // 4
	uint16_t fatherindex;           // 2
}A_Star_Node;                       // = 12

// Queue
typedef struct
{
	uint16_t Head;                    // 2
	uint16_t Length;                  // 2
	A_Star_Node buffer[QUEUE_SIZE];   // 128 * 12
}Queue;                            	  // 128 * 12 + 4

// List
typedef struct
{
	uint16_t Head;                    // 2
	uint16_t Tail;					  // 2
	uint16_t Length;                  // 2
	uint16_t buffer[LIST_SIZE];       // 128 * 2
}List;                                // 128 * 2 + 6
#pragma pack ()

void Queue_init(void);
void List_init(void);

uint8_t queue_append(A_Star_Node* node);
uint8_t list_append(uint16_t c);

uint16_t queue_top();
uint16_t findin_queue(uint16_t c);
uint8_t findin_list(uint16_t c);

void editcost(uint16_t index, A_Star_Node* node);

#endif /* INC_MING_MALLOC_H_ */
