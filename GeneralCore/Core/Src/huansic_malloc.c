/*
 * huansic_malloc.c
 *
 *  Created on: Oct 19, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_malloc.h"

#ifndef HUANSIC_PATH_MALLOC_SIZE
#define HUANSIC_PATH_PREMALLOC_SIZE 32
#endif

Path pathBuffers[HUANSIC_PATH_PREMALLOC_SIZE];

__weak void custom_path_free_fault(Path *ptr) {

}

void huansic_path_malloc_init() {
	uint8_t i;
	for (i = 0; i < HUANSIC_PATH_PREMALLOC_SIZE; i++) {
		pathBuffers[i].referenceCount = 0;	// clear referenced counter so that it can be malloced
		pathBuffers[i].nextPath = 0;		// set to nullptr just in case
	}
}

Path* huansic_path_new() {
	Path *retPtr = 0;
	uint8_t i;
	for (i = 0; i < HUANSIC_PATH_PREMALLOC_SIZE; i++) {
		if (pathBuffers[i].referenceCount == 0) {
			pathBuffers[i].referenceCount = 1;
			retPtr = &pathBuffers[i];
			break;		// could've just returned the pointer, but to keep it consistent...
		}
	}

	return retPtr;
}

void huansic_path_delete(Path *ptr) {
	uint8_t i;
	for (i = 0; i < HUANSIC_PATH_PREMALLOC_SIZE; i++) {
		if (pathBuffers[i].nextPath == ptr) {			// clear all references in the list
			ptr->referenceCount--;
			pathBuffers[i].nextPath = 0;
		}
	}

	if (ptr->referenceCount != 1) {
		custom_path_free_fault(ptr);		// if there're other references, run the ISR
	}

	// and free itself
	ptr->referenceCount = 0;
}

void huansic_path_cascade(Path *last, Path *next) {
	if (!last)		// make "next" head; no actions
		return;

	if (last->nextPath != next) {
		if (!last->nextPath)
			last->nextPath->referenceCount--;
		last->nextPath = next;

		if (next)		// if next is non-zero, make sure to increase the reference count
			next->referenceCount++;
	}
}

Path* huansic_path_break(Path *newTail) {
	if (!newTail)
		return 0;

	Path *retPath = newTail->nextPath;		// return the new head (if any)
	if (retPath) {
		retPath->referenceCount--;
		newTail->nextPath = 0;
	}
	return retPath;
}
