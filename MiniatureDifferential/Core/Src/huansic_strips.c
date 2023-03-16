/*
 * huansic_strips.c
 *
 *  Created on: Mar 9, 2023
 *      Author: abcde
 */

#include "huansic_strips.h"

unsigned short pallets[172][5];		// 5 obstacles
extern Rectangle obstacles[11];		// two extra for safety

void strip_init() {
	unsigned char i, j;
	for (i = 0; i < 5; i++)
		for (j = 0; j < 164; j++)
			pallets[j][i] = 0;
}

void strip_addRect(Rectangle *rect) {
	unsigned char i, row, start, end, tempstart, tempend;
	unsigned short temps;
	for (row = rect->y1; row < rect->y2; row++) {
		start = rect->x1;
		end = rect->x2;
		for (i = 0; i < 5; i++) {
			if (pallets[row - 41][i]) {
				tempstart = (pallets[row - 41][i] >> 8) & 0x00FF;
				tempend = pallets[row - 41][i] & 0x00FF;
				if (tempstart <= end && tempstart > start) {	// cascade to left
					temps = start;
					temps <<= 8;
					temps |= tempend;
					pallets[row - 41][i] = temps;
				} else if (tempend < end && tempend >= start) {	// cascade to right
					temps = tempstart;
					temps <<= 8;
					temps |= end;
					pallets[row - 41][i] = temps;
				} else if (tempstart <= start && tempend >= end) {	// keep original one
					;
				} else if (start < tempstart && end > tempend) {		// keep new one
					temps = start;
					temps <<= 8;
					temps |= end;
					pallets[row - 41][i] = temps;
				} else {
					continue;
				}
			} else {
				temps = start;
				temps <<= 8;
				temps |= end;
				pallets[row - 41][i] = temps;
			}
			break;
		}
	}
}

// there's really no use for this
void strip_sort() {
	unsigned char i, tempstart1, tempend1, tempstart2, tempend2;
	unsigned short temp;
	for (i = 0; i < 164; i++) {
		// first sort by start
		if (pallets[i][4] && ((pallets[i][4] & 0xFF00) < (pallets[i][3] & 0xFF00))) {
			temp = pallets[i][4];
			pallets[i][4] = pallets[i][3];
			pallets[i][3] = temp;
		}
		if (pallets[i][3] && ((pallets[i][3] & 0xFF00) < (pallets[i][2] & 0xFF00))) {
			temp = pallets[i][3];
			pallets[i][3] = pallets[i][2];
			pallets[i][2] = temp;
		}
		if (pallets[i][2] && ((pallets[i][2] & 0xFF00) < (pallets[i][1] & 0xFF00))) {
			temp = pallets[i][2];
			pallets[i][2] = pallets[i][1];
			pallets[i][1] = temp;
		}
		if (pallets[i][1] && ((pallets[i][1] & 0xFF00) < (pallets[i][0] & 0xFF00))) {
			temp = pallets[i][1];
			pallets[i][1] = pallets[i][0];
			pallets[i][0] = temp;
		}

		// then attemp to combine
		tempstart1 = (pallets[i][0] >> 8) & 0x00FF;
		tempend1 = pallets[i][0] & 0x00FF;
		tempstart2 = (pallets[i][1] >> 8) & 0x00FF;
		tempend2 = pallets[i][1] & 0x00FF;
		if (tempend1 <= tempstart2) {
			temp = tempstart1;
			temp <<= 8;
			temp |= tempend2;
			pallets[i][1] = temp;
		}
		tempstart1 = (pallets[i][1] >> 8) & 0x00FF;
		tempend1 = pallets[i][1] & 0x00FF;
		tempstart2 = (pallets[i][2] >> 8) & 0x00FF;
		tempend2 = pallets[i][2] & 0x00FF;
		if (tempend1 <= tempstart2) {
			temp = tempstart1;
			temp <<= 8;
			temp |= tempend2;
			pallets[i][2] = temp;
		}
		tempstart1 = (pallets[i][2] >> 8) & 0x00FF;
		tempend1 = pallets[i][2] & 0x00FF;
		tempstart2 = (pallets[i][3] >> 8) & 0x00FF;
		tempend2 = pallets[i][3] & 0x00FF;
		if (tempend1 <= tempstart2) {
			temp = tempstart1;
			temp <<= 8;
			temp |= tempend2;
			pallets[i][3] = temp;
		}
		tempstart1 = (pallets[i][3] >> 8) & 0x00FF;
		tempend1 = pallets[i][3] & 0x00FF;
		tempstart2 = (pallets[i][4] >> 8) & 0x00FF;
		tempend2 = pallets[i][4] & 0x00FF;
		if (tempend1 <= tempstart2) {
			temp = tempstart1;
			temp <<= 8;
			temp |= tempend2;
			pallets[i][4] = temp;
		}

		// fill the gaps at the front; just brute force it
		if (!pallets[i][0]) {
			if (pallets[i][1]) {
				pallets[i][0] = pallets[i][1];
				pallets[i][1] = 0;
			} else if (pallets[i][2]) {
				pallets[i][0] = pallets[i][2];
				pallets[i][2] = 0;
			} else if (pallets[i][3]) {
				pallets[i][0] = pallets[i][3];
				pallets[i][3] = 0;
			} else if (pallets[i][4]) {
				pallets[i][0] = pallets[i][4];
				pallets[i][4] = 0;
			}
		}
		if (!pallets[i][1]) {
			if (pallets[i][2]) {
				pallets[i][1] = pallets[i][2];
				pallets[i][2] = 0;
			} else if (pallets[i][3]) {
				pallets[i][1] = pallets[i][3];
				pallets[i][3] = 0;
			} else if (pallets[i][4]) {
				pallets[i][1] = pallets[i][4];
				pallets[i][4] = 0;
			}
		}
		if (!pallets[i][2]) {
			if (pallets[i][3]) {
				pallets[i][2] = pallets[i][3];
				pallets[i][3] = 0;
			} else if (pallets[i][4]) {
				pallets[i][2] = pallets[i][4];
				pallets[i][4] = 0;
			}
		}
		if (!pallets[i][3]) {
			if (pallets[i][4]) {
				pallets[i][3] = pallets[i][4];
				pallets[i][4] = 0;
			}
		}
	}
}

unsigned char strip_toRectangle() {
	unsigned char i, j, k = 0;
	unsigned char forming = 0;
	unsigned char currentstart, currentend, tempstart, tempend;

	do {
		forming = 0;
		for (i = 0; i < 172; i++) {
			// search vertically
			for (j = 0; j < 5; j++) {
				// search laterally
				if (pallets[i][j]) {
					// if non-zero
					tempstart = (pallets[i][j] >> 8) & 0x00FF;
					tempend = pallets[i][j] & 0x00FF;
					if (forming) {
						// check for alignment
						if (currentstart == tempstart && currentend == tempend) {
							// if it aligns, good
							pallets[i][j] = 0;
							break;
						} else {
							// continue;
						}
					} else {
						// first hit on this scan
						// then create a new rectangle
						currentstart = tempstart;
						currentend = tempend;
						forming = 1;
						pallets[i][j] = 0;
						obstacles[k].x1 = currentstart;
						obstacles[k].x2 = currentend;
						obstacles[k].y1 = i + 41;
						break;
					}
				} else {
					// if empty, check the next one
					// contnue;
				}
			}
			if (j == 5 && forming) {
				// if nothing is matched but a rectangle is forming
				// exit the loop to form a rectangle
				break;
			} else {
				// continue;
			}
		}
		if (forming) {
			// if a rectangle has formed
			// then finish the rectangle
			obstacles[k].y2 = i + 41;

			// and prepare for the next one
			k++;
		} else {
			// if all pallets are clear
			// it should exit the while loop immediately
		}
	} while (forming);

	return k;
}
