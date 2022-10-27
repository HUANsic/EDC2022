/*
 * huansic_xblib.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Zonghuan Wu
 */

#include "huansic_xblib.h"

// game information 1
extern uint8_t gameStage;			// 0: pre-match(standby); 1: first half; 2: second half
extern uint8_t gameStatus;			// 0: standby; 1: running
extern uint32_t gameStageTimeLimit;		// in ms
extern uint32_t gameStageTimeSinceStart;	// in ms
extern Rectangle obstacles[5];			// area that depletes charge faster
extern Coordinate allyBeacons[3];		// ally charging station coordinate
extern Coordinate oppoBeacons[3];		// opponent charging station coordinate

// game information 2
extern Order *delivering[5];		// package picked up but not yet delivered
extern Order orders[60];			// package that has not yet been picked up

// game information 3
extern Coordinate myCoord;			// precise coordinate returned by game master
extern fCoordinate estimatedCoord;	// coordinate calculated by Kalman Filter
extern double omegaZ, accelY;		// turning speed and linear acceleration
extern float myScore;				// current score returned by Master
extern float myCharge;				// current charge returned by Master

// interchange information 1
extern uint32_t gameStageTimeLeft;		// in ms

void huansic_xb_init(XB_HandleTypeDef *hxb) {
	hxb->nextPackageLength = 6;		// header length
	HAL_UART_Receive_DMA(hxb->uartPort, hxb->buffer, hxb->nextPackageLength);
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
	uint8_t listLength = 0, i, j, index = 0;
	uint32_t temp;
	if (hxb->nextPackageID == 0x01) {		// game information
		/* game stage */
		gameStage = hxb->buffer[index++];

		/* barrier list */
		// listLength = hxb->buffer[index];		// the length is fixed to 5
		for (i = 0; i < 5; i++) {
			index += 4;
			obstacles[i].coord1.x = hxb->buffer[index];
			index += 4;
			obstacles[i].coord1.y = hxb->buffer[index];
			index += 4;
			obstacles[i].coord2.x = hxb->buffer[index];
			index += 4;
			obstacles[i].coord2.y = hxb->buffer[index];
		}

		/* total time of this round */
		index++;
		gameStageTimeLimit = hxb->buffer[index++];
		gameStageTimeLimit <<= 8;
		gameStageTimeLimit |= hxb->buffer[index++];
		gameStageTimeLimit <<= 8;
		gameStageTimeLimit |= hxb->buffer[index++];
		gameStageTimeLimit <<= 8;
		gameStageTimeLimit |= hxb->buffer[index++];

		/* ally beacons */
		listLength = hxb->buffer[index];
		for (i = 0; i < listLength; i++) {
			index += 4;
			allyBeacons[i].x = hxb->buffer[index];
			index += 4;
			allyBeacons[i].y = hxb->buffer[index];
		}

		/* opponent beacons */
		index++;
		listLength = hxb->buffer[index];
		for (i = 0; i < listLength; i++) {
			index += 4;
			allyBeacons[i].x = hxb->buffer[index];
			index += 4;
			allyBeacons[i].y = hxb->buffer[index];
		}
	} else if (hxb->nextPackageID == 0x05) {		// game status
		/* game status */
		gameStatus = hxb->buffer[index++];

		/* time since round started */
		gameStageTimeSinceStart = hxb->buffer[index++];
		gameStageTimeSinceStart <<= 8;
		gameStageTimeSinceStart |= hxb->buffer[index++];
		gameStageTimeSinceStart <<= 8;
		gameStageTimeSinceStart |= hxb->buffer[index++];
		gameStageTimeSinceStart <<= 8;
		gameStageTimeSinceStart |= hxb->buffer[index++];
		gameStageTimeLeft = gameStageTimeLimit - gameStageTimeSinceStart;

		/* fetch score */
		temp = hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		myScore = *(float*) &temp;			// decode float from uint32

		/* my position */
		index += 3;
		myCoord.x = hxb->buffer[index++];
		index += 3;
		myCoord.y = hxb->buffer[index++];

		/* fetch battery */
		temp = hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		temp <<= 8;
		temp |= hxb->buffer[index++];
		myCharge = *(float*) &temp;			// decode float from uint32

		/* my orders */
		listLength = hxb->buffer[index++];
		for (i = 0; i < listLength; i++) {
			temp = hxb->buffer[index + 24];
			temp <<= 8;
			temp |= hxb->buffer[index + 25];
			temp <<= 8;
			temp |= hxb->buffer[index + 26];
			temp <<= 8;
			temp |= hxb->buffer[index + 27];
			Order *tempOrder = huansic_order_new(temp);

			tempOrder->startCoord.x = hxb->buffer[index + 3];
			tempOrder->startCoord.y = hxb->buffer[index + 7];

			tempOrder->destCoord.x = hxb->buffer[index + 11];
			tempOrder->destCoord.y = hxb->buffer[index + 15];

			temp = hxb->buffer[index + 16];
			temp <<= 8;
			temp |= hxb->buffer[index + 17];
			temp <<= 8;
			temp |= hxb->buffer[index + 18];
			temp <<= 8;
			temp |= hxb->buffer[index + 19];
			tempOrder->timeLimit = temp;

			temp = hxb->buffer[index + 20];
			temp <<= 8;
			temp |= hxb->buffer[index + 21];
			temp <<= 8;
			temp |= hxb->buffer[index + 22];
			temp <<= 8;
			temp |= hxb->buffer[index + 23];
			tempOrder->reward = *(float*) &temp;

			index += 28;
		}
		// TODO order management
	}

	// set up next DMA
	hxb->nextPackageLength = 6;		// header length
	HAL_UART_Receive_DMA(hxb->uartPort, hxb->buffer, hxb->nextPackageLength);
}

void huansic_xb_requestGameInfo(XB_HandleTypeDef *hxb) {
	uint8_t buffer = 0x00;
	HAL_UART_Transmit(hxb->uartPort, &buffer, 1, 10);
}

void huansic_xb_setBeacon(XB_HandleTypeDef *hxb) {
	uint8_t buffer = 0x02;
	HAL_UART_Transmit(hxb->uartPort, &buffer, 1, 10);
}
