/*
 * ming_types.h
 *
 *  Created on: 2022��11��13��
 *      Author: Ming
 */

#ifndef INC_MING_TYPES_H_
#define INC_MING_TYPES_H_

// #include "stm32f1xx_hal.h"
# include<stdio.h>
# include<stdlib.h>
# include<stdint.h>


typedef struct A_Star_Node_t{
	uint16_t x, y;                   // 2 + 2
	uint16_t Cost, Total;            // 2 + 2
	struct A_Star_Node_t* father;    // 4
	struct A_Star_Node_t* next;      // 4
} A_Star_Node;                       // = 16

typedef struct {
	uint8_t map_icon[256][256];      // 256 * 256 = 65536
} Map;

#endif /* INC_MING_TYPES_H_ */
