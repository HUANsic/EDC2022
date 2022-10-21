/*
 * huansic_xblib.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_xblib.h"

extern uint8_t gameStatus;

void huansic_xb_init(XB_HandleTypeDef *hxb) {

}

uint8_t huansic_xb_decodeHeader(XB_HandleTypeDef *hxb) {
	// checksum
	if (hxb->buffer[5]
			!= (hxb->buffer[0] ^ hxb->buffer[1] ^ hxb->buffer[2] ^ hxb->buffer[3] ^ hxb->buffer[4]))
		return 0;

	// get and check packet ID
	if (hxb->buffer[0] != 0x01 && hxb->buffer[0] != 0x05)
		return 0;
	hxb->nextPackageID = hxb->buffer[0];

	// read next package length
	hxb->nextPackageLength = hxb->buffer[4]; // the length shall not be longer than 255 (the max possible is 225)

	// set up next DMA
	HAL_UART_Receive_DMA(hxb->uartPort, hxb->buffer, hxb->nextPackageLength);
	return 1;
}

void huansic_xb_decodeBody(XB_HandleTypeDef *hxb) {
	if (hxb->nextPackageID == 0x01) {

	} else if (hxb->nextPackageID == 0x05) {

	}

	// set up next DMA
	hxb->nextPackageLength = 6;		// header length
	HAL_UART_Receive_DMA(hxb->uartPort, hxb->buffer, hxb->nextPackageLength);
}
