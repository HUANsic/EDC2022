/*
 * huansic_xblib.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_XBLIB_H_
#define INC_HUANSIC_XBLIB_H_

#include "huansic_types.h"
#include "huansic_malloc.h"

typedef struct {
	UART_HandleTypeDef *uartPort;

	uint32_t lastUpdated;

	// for internal use; user shall not modify this
	uint8_t nextPackageID;			// 0x00: next is header; others: check ID
	uint8_t nextPackageLength;		// length of next DMA receive

	// max is 126
	uint8_t buffer[130];	// put at the end to prevent block alignment issues
} XB_HandleTypeDef;

void huansic_xb_init(XB_HandleTypeDef *hxb);

void huansic_xb_align(XB_HandleTypeDef *hxb);

uint8_t huansic_xb_decodeHeader(XB_HandleTypeDef *hxb);

void huansic_xb_decodeBody(XB_HandleTypeDef *hxb);

void huansic_xb_decodeRemote(XB_HandleTypeDef *hxb);

void huansic_xb_requestGameInfo(XB_HandleTypeDef *hxb);

void huansic_xb_setBeacon(XB_HandleTypeDef *hxb);

void huansic_xb_interact(XB_HandleTypeDef *hxb);

#endif /* INC_HUANSIC_XBLIB_H_ */
