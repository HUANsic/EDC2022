/*
 * huansic_strips.h
 *
 *  Created on: Mar 9, 2023
 *      Author: abcde
 */

#ifndef INC_HUANSIC_STRIPS_H_
#define INC_HUANSIC_STRIPS_H_

#include "huansic_types.h"

void strip_init();

void strip_addRect(Rectangle *rect);

void strip_sort();

unsigned char strip_toRectangle();

#endif /* INC_HUANSIC_STRIPS_H_ */
