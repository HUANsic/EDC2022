/*
 * huansic_jy62lib.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_JY62LIB_H_
#define INC_HUANSIC_JY62LIB_H_

#include "stm32f1xx_hal.h"
#include <assert.h>
#include <math.h>
#include "huansic_types.h"

void huansic_jy62_init(JY62_HandleTypeDef *hjy62);

void huansic_jy62_resetAngle(JY62_HandleTypeDef *hjy62);

void huansic_jy62_decodePackage(JY62_HandleTypeDef *hjy62);

void huansic_jy62_setDMA(JY62_HandleTypeDef *hjy62);

#endif /* INC_HUANSIC_JY62LIB_H_ */
