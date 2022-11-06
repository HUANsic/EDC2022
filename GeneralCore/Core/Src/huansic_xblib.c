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

__weak void custom_order_new_failed(uint8_t id) {

}

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
		index++;
		for (i = 0; i < 5; i++) {
			index += 2;
			obstacles[i].coord1.x = (uint16_t) hxb->buffer[index++] << 8;
			obstacles[i].coord1.x = hxb->buffer[index++];
			index += 2;
			obstacles[i].coord1.y = (uint16_t) hxb->buffer[index++] << 8;
			obstacles[i].coord1.y = hxb->buffer[index];
			index += 2;
			obstacles[i].coord2.x = (uint16_t) hxb->buffer[index++] << 8;
			obstacles[i].coord2.x = hxb->buffer[index];
			index += 2;
			obstacles[i].coord2.y = (uint16_t) hxb->buffer[index++] << 8;
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
			index += 2;
			allyBeacons[i].x = (uint16_t) hxb->buffer[index++] << 8;
			allyBeacons[i].x = hxb->buffer[index++];
			index += 2;
			allyBeacons[i].y = (uint16_t) hxb->buffer[index++] << 8;
			allyBeacons[i].y = hxb->buffer[index++];
		}

		/* opponent beacons */
		index++;
		listLength = hxb->buffer[index];
		for (i = 0; i < listLength; i++) {
			index += 2;
			oppoBeacons[i].x = (uint16_t) hxb->buffer[index++] << 8;
			oppoBeacons[i].x = hxb->buffer[index++];
			index += 2;
			oppoBeacons[i].y = (uint16_t) hxb->buffer[index++] << 8;
			oppoBeacons[i].y = hxb->buffer[index++];
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
		index += 2;
		myCoord.x = (uint16_t) hxb->buffer[index++] << 8;
		myCoord.x = hxb->buffer[index++];
		index += 2;
		myCoord.y = (uint16_t) hxb->buffer[index++] << 8;
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
		int8_t updatedOrder[] = { -1, -1, -1, -1, -1 };
		uint8_t updatedOrderIndex = 0;
		Order *tempOrder;
		listLength = hxb->buffer[index++];
		for (i = 0; i < listLength; i++) {
			temp = hxb->buffer[index + 24];
			temp <<= 8;
			temp |= hxb->buffer[index + 25];
			temp <<= 8;
			temp |= hxb->buffer[index + 26];
			temp <<= 8;
			temp |= hxb->buffer[index + 27];
			tempOrder = huansic_order_new(temp);
			if (!tempOrder) {
				index += 28;
				custom_order_new_failed(temp);
				continue;
			}
			// start coordinate
			tempOrder->startCoord.x = ((uint16_t) hxb->buffer[index + 2] << 8)
					| hxb->buffer[index + 3];
			tempOrder->startCoord.y = ((uint16_t) hxb->buffer[index + 6] << 8)
					| hxb->buffer[index + 7];
			// destination
			tempOrder->destCoord.x = ((uint16_t) hxb->buffer[index + 10] << 8)
					| hxb->buffer[index + 11];
			tempOrder->destCoord.y = ((uint16_t) hxb->buffer[index + 14] << 8)
					| hxb->buffer[index + 15];
			// time limit
			temp = hxb->buffer[index + 16];
			temp <<= 8;
			temp |= hxb->buffer[index + 17];
			temp <<= 8;
			temp |= hxb->buffer[index + 18];
			temp <<= 8;
			temp |= hxb->buffer[index + 19];
			tempOrder->timeLimit = temp;
			// reward
			temp = hxb->buffer[index + 20];
			temp <<= 8;
			temp |= hxb->buffer[index + 21];
			temp <<= 8;
			temp |= hxb->buffer[index + 22];
			temp <<= 8;
			temp |= hxb->buffer[index + 23];
			tempOrder->reward = *(float*) &temp;
			// increment index and record id
			index += 28;
			updatedOrder[updatedOrderIndex++] = tempOrder->id;
		}

		/* order management */
		for (i = 0; i < 5; i++)
			if (delivering[i]->id != -1) {
				for (j = 0; i < updatedOrderIndex; j++)
					if (delivering[i]->id == updatedOrder[j]) {		// pulled from remote
						j = 255;
						break;
					}
				if (j != 255)
					huansic_order_delete(delivering[i]);// delete the order if the order is no longer in the delivery list
			}

		/* record latest order */
		temp = hxb->buffer[index + 24];
		temp <<= 8;
		temp |= hxb->buffer[index + 25];
		temp <<= 8;
		temp |= hxb->buffer[index + 26];
		temp <<= 8;
		temp |= hxb->buffer[index + 27];
		tempOrder = huansic_order_new(temp);
		if (!tempOrder) {
			index += 28;
			custom_order_new_failed(temp);
		} else {
			// start coordinate
			tempOrder->startCoord.x = ((uint16_t) hxb->buffer[index + 2] << 8)
					| hxb->buffer[index + 3];
			tempOrder->startCoord.y = ((uint16_t) hxb->buffer[index + 6] << 8)
					| hxb->buffer[index + 7];
			// end coordinate
			tempOrder->destCoord.x = ((uint16_t) hxb->buffer[index + 10] << 8)
					| hxb->buffer[index + 11];
			tempOrder->destCoord.y = ((uint16_t) hxb->buffer[index + 14] << 8)
					| hxb->buffer[index + 15];
			// time limit
			temp = hxb->buffer[index + 16];
			temp <<= 8;
			temp |= hxb->buffer[index + 17];
			temp <<= 8;
			temp |= hxb->buffer[index + 18];
			temp <<= 8;
			temp |= hxb->buffer[index + 19];
			tempOrder->timeLimit = temp;
			// reward
			temp = hxb->buffer[index + 20];
			temp <<= 8;
			temp |= hxb->buffer[index + 21];
			temp <<= 8;
			temp |= hxb->buffer[index + 22];
			temp <<= 8;
			temp |= hxb->buffer[index + 23];
			tempOrder->reward = *(float*) &temp;
		}
	}

	// set up next DMA
	hxb->nextPackageLength = 6;		// header length
	hxb->nextPackageID = 0x00;		// the next one is header
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
