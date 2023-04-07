/*
 * huansic_jy62_it.h	version 2
 *
 *  Created on: Apr 24, 2023
 *      Author: Zonghuan Wu
 */

#ifndef INC_HUANSIC_JY62LIB_H_
#define INC_HUANSIC_JY62LIB_H_

#include "stm32f1xx_hal.h"
#include "huansic_types.h"

/*
 * 		Initializes the port of the IMU.
 * 		@param	himu	jy62 pending initialization
 * 		@retval	enum IMU_STATUS
 */
enum IMU_STATUS huansic_jy62_init_dma(JY62_HandleTypeDef *himu);

/*
 * 		Handles the dma interrupts. It only happens when all 33 bytes are received.
 * 		@param	himu	jy62 whose port has sent out the interrupt signal
 * 		@retval	enum IMU_STATUS
 */
enum IMU_STATUS huansic_jy62_dma_isr(JY62_HandleTypeDef *himu);

/*
 * 		Handles the line idle interrupts.
 * 		@param	himu	jy62 whose port has sent out the interrupt signal
 * 		@retval	enum IMU_STATUS
 */
void huansic_jy62_idle_isr(JY62_HandleTypeDef *himu);

/*
 * 		Handles the errors.
 * 		@param	himu	jy62 whose port has sent out the error
 * 		@retval	enum IMU_STATUS
 */
void huansic_jy62_error_isr(JY62_HandleTypeDef *himu);

#endif /* INC_HUANSIC_JY62LIB_H_ */
