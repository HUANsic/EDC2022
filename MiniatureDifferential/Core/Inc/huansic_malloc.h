/*
 * huansic_malloc.h
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_MALLOC_H_
#define INC_HUANSIC_MALLOC_H_

#include "huansic_types.h"

void huansic_path_malloc_init();

Path* huansic_path_new();

void huansic_path_delete(Path *ptr);

void huansic_path_cascade(Path *last, Path *next);

Path* huansic_path_break(Path *newTail);

#endif /* INC_HUANSIC_MALLOC_H_ */