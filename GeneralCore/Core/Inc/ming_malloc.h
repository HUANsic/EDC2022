/*
 * ming_malloc.h
 *
 *  Created on: 2022年11月21日
 *      Author: Ming
 */

#ifndef INC_MING_MALLOC_H_
#define INC_MING_MALLOC_H_

#include "stm32f1xx_hal.h"
#include "huansic_types.h"
#include "huansic_malloc.h"
#include "stdlib.h"

#define QUEUE_SIZE 2048
#define LIST_SIZE 2048
#define LANE_SIZE 32

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
	A_Star_Node buffer[QUEUE_SIZE];   // QUEUE_SIZE * 12
}Queue;                            	  // QUEUE_SIZE * 12 + 4

// List
typedef struct
{
	uint16_t Head;                    // 2
	uint16_t Tail;					  // 2
	uint16_t Length;                  // 2
	uint16_t buffer[LIST_SIZE];       // LIST_SIZE * 2
}List;                                // LIST_SIZE * 2 + 6

typedef struct
{
	uint16_t Head;                    // 2
	uint16_t Length;                  // 2
	Coordinate buffer[LANE_SIZE];     // 4 * LANE_SIZE
}Lane;                                // 4 * LANE_SIZE + 4

typedef struct
{
	uint8_t length;
	uint8_t new;
	Coordinate buffer[60];
}Order_list;

#pragma pack ()

void Queue_init(void);
void List_init(void);
void Lane_init(void);

uint8_t queue_append(A_Star_Node* node);
uint8_t list_append(uint16_t c);

uint16_t queue_top();
uint16_t findin_queue(uint16_t c);
uint16_t findendin_queue(uint16_t c);
uint8_t findin_list(uint16_t c);

void editcost(uint16_t index, A_Star_Node* node);

// Lane part
uint8_t Insert_inLane(Coordinate *head_coor, uint8_t head_index);

//order list
void order_list_init(void);
void order_append(Order *an_order);
int8_t Get_nearest_order(void);

void exitpoints_init(void);
#endif /* INC_MING_MALLOC_H_ */
