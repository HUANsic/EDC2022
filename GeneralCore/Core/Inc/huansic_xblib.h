/*
 * huansic_xblib.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_XBLIB_H_
#define INC_HUANSIC_XBLIB_H_

#include "huansic_types.h"

void huansic_xb_init(XB_HandleTypeDef *hxb);

uint8_t huansic_xb_decodeHeader(XB_HandleTypeDef *hxb);

void huansic_xb_decodeBody(XB_HandleTypeDef *hxb);

void huansic_xb_requestGameInfo(XB_HandleTypeDef *hxb);

void huansic_xb_setBeacon(XB_HandleTypeDef *hxb);

#endif /* INC_HUANSIC_XBLIB_H_ */
