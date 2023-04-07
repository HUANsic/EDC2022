/*
 * huansic_jy62_it.c	version 2
 *
 *  Created on: Apr 24, 2023
 *      Author: Zonghuan Wu
 */

#include <huansic_jy62lib.h>

#define	HUANSIC_JY62_HEADER		0x55
#define	HUANSIC_JY62_ACCEL		0x51
#define	HUANSIC_JY62_OMEGA		0x52
#define	HUANSIC_JY62_THETA		0x53

uint8_t JY62_RESET_Z_ANGLE[] = { 0xFF, 0xAA, 0x52 };
uint8_t JY62_BAUD_115200[] = { 0xFF, 0xAA, 0x63 };
uint8_t JY62_BAUD_9600[] = { 0xFF, 0xAA, 0x64 };

// private function prototypes
static inline void __huansic_jy62_decode_accel(JY62_HandleTypeDef *himu, uint8_t location);
static inline void __huansic_jy62_decode_omega(JY62_HandleTypeDef *himu, uint8_t location);
static inline void __huansic_jy62_decode_theta(JY62_HandleTypeDef *himu, uint8_t location);
static inline uint8_t __huansic_jy62_checksum(JY62_HandleTypeDef *himu, uint8_t location);

/*
 * 		Initializes the port of the IMU.
 * 		@param	himu	jy62 pending initialization
 * 		@retval	enum IMU_STATUS
 */
enum IMU_STATUS huansic_jy62_init_dma(JY62_HandleTypeDef *himu) {
	// perform some necessary checks
	if (!himu)
		return IMU_ERROR;

	if (!himu->huart)
		return IMU_ERROR;

	HAL_Delay(3);

#ifdef JY62_RATE_100HZ
	if (himu->huart->Init.BaudRate != 9600) {		// if it is not 9600bps
		// make it 9600 to send the package
		HAL_UART_DeInit(himu->huart);
		himu->huart->Init.BaudRate = 9600;
		HAL_UART_Init(himu->huart);
	}
	HAL_UART_Transmit(himu->huart, JY62_BAUD_115200, 3, 10);
	HAL_UART_DeInit(himu->huart);
	himu->huart->Init.BaudRate = 115200;
	HAL_UART_Init(himu->huart);
#else
#ifdef JY62_RATE_20HZ
	if (himu->huart->Init.BaudRate != 115200) {		// if it is not 115200bps
		// make it 115200 to send the package
		HAL_UART_DeInit(himu->huart);
		himu->huart->Init.BaudRate = 115200;
		HAL_UART_Init(himu->huart);
	}
	HAL_UART_Transmit(himu->huart, JY62_BAUD_9600, 3, 10);
	HAL_UART_DeInit(himu->huart);
	himu->huart->Init.BaudRate = 9600;
	HAL_UART_Init(himu->huart);
#endif	// JY62_RATE_20HZ
#endif	// JY62_RATE_100HZ

	// reset z-axis angle
	HAL_UART_Transmit(himu->huart, JY62_RESET_Z_ANGLE, 3, 10);

	// find head
	HAL_UART_Receive_DMA(himu->huart, himu->buffer, 33);
	__HAL_UART_DISABLE_IT(himu->huart, DMA_IT_HT);		// disable half transfer interrupt
#ifndef JY62_MINIMAL
	himu->dma_hw_error_counter = 0;
	himu->it_hw_error_counter = 0;
	himu->pid_error_counter = 0;
	himu->checksum_error_counter = 0;
	himu->header_error_counter = 0;
	himu->success_counter = 0;
	himu->accel_success_counter = 0;
	himu->omega_success_counter = 0;
	himu->theta_success_counter = 0;
	himu->recover_counter = 0;
#endif
	return IMU_OK;
}

/*
 * 		Handles the dma interrupts. It only happens when all 33 bytes are received.
 * 		@param	himu	jy62 whose port has sent out the interrupt signal
 * 		@retval	enum IMU_STATUS
 */
enum IMU_STATUS huansic_jy62_dma_isr(JY62_HandleTypeDef *himu) {
	if (!himu)
		return IMU_ERROR;

	uint8_t errorcode0 = IMU_OK, errorcode1 = IMU_OK, errorcode2 = IMU_OK;

	// check received length
	if ((himu->buffer[0] == HUANSIC_JY62_HEADER) && (himu->buffer[11] == HUANSIC_JY62_HEADER)
			&& (himu->buffer[22] == HUANSIC_JY62_HEADER)) {
		if (__huansic_jy62_checksum(himu, 0)) {
			if (himu->buffer[1] == HUANSIC_JY62_ACCEL) {
				__huansic_jy62_decode_accel(himu, 0);
			} else if (himu->buffer[1] == HUANSIC_JY62_OMEGA) {
				__huansic_jy62_decode_omega(himu, 0);
			} else if (himu->buffer[1] == HUANSIC_JY62_THETA) {
				__huansic_jy62_decode_theta(himu, 0);
			} else {
#ifndef JY62_MINIMAL
				himu->pid_error_counter++;
#endif
				errorcode0 = IMU_PID_ERROR;
			}
		} else {
#ifndef JY62_MINIMAL
			himu->checksum_error_counter++;
#endif
			errorcode0 = IMU_SUM_ERROR;
		}	// end if (checksum, 0)
/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (__huansic_jy62_checksum(himu, 1)) {
			if (himu->buffer[12] == HUANSIC_JY62_ACCEL) {
				__huansic_jy62_decode_accel(himu, 1);
			} else if (himu->buffer[12] == HUANSIC_JY62_OMEGA) {
				__huansic_jy62_decode_omega(himu, 1);
			} else if (himu->buffer[12] == HUANSIC_JY62_THETA) {
				__huansic_jy62_decode_theta(himu, 1);
			} else {
#ifndef JY62_MINIMAL
				himu->pid_error_counter++;
#endif
				errorcode1 = IMU_PID_ERROR;
			}
		} else {
#ifndef JY62_MINIMAL
			himu->checksum_error_counter++;
#endif
			errorcode1 = IMU_SUM_ERROR;
		}	// end if (checksum, 1)
/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (__huansic_jy62_checksum(himu, 2)) {
			if (himu->buffer[23] == HUANSIC_JY62_ACCEL) {
				__huansic_jy62_decode_accel(himu, 2);
			} else if (himu->buffer[23] == HUANSIC_JY62_OMEGA) {
				__huansic_jy62_decode_omega(himu, 2);
			} else if (himu->buffer[23] == HUANSIC_JY62_THETA) {
				__huansic_jy62_decode_theta(himu, 2);
			} else {
#ifndef JY62_MINIMAL
				himu->pid_error_counter++;
#endif
				errorcode2 = IMU_PID_ERROR;
			}
		}
		else {
#ifndef JY62_MINIMAL
			himu->checksum_error_counter++;
#endif
			errorcode2 = IMU_SUM_ERROR;
		}	// end if (checksum, 2)
	}
	else {	// just drop the entire package, make sure all fields are up to date at the same frame
#ifndef JY62_MINIMAL
		himu->header_error_counter++;
#endif
		HAL_UARTEx_ReceiveToIdle_DMA(himu->huart, himu->buffer, 33);
		__HAL_UART_DISABLE_IT(himu->huart, DMA_IT_HT);		// disable half transfer interrupt
		return IMU_HEADER_ERROR;
	}	// end else (size)

	// start to receive the next package
	HAL_UART_Receive_DMA(himu->huart, himu->buffer, 33);
	__HAL_UART_DISABLE_IT(himu->huart, DMA_IT_HT);		// disable half transfer interrupt
#ifndef JY62_MINIMAL
	himu->success_counter = himu->accel_success_counter + himu->omega_success_counter
			+ himu->theta_success_counter;
	HAL_GPIO_WritePin(himu->port, himu->pin, himu->success_counter & (1 << 5));
#endif

	// update error code
	errorcode0 =
			(errorcode0 == IMU_OK) ?
					((errorcode1 == IMU_OK) ? ((errorcode2 == HAL_OK) ? HAL_OK : errorcode2) :
												errorcode1) :
					errorcode0;
	return errorcode0;
}

/*
 * 		Handles the line idle interrupts.
 * 		@param	himu	jy62 whose port has sent out the interrupt signal
 * 		@retval	enum IMU_STATUS
 */
void huansic_jy62_idle_isr(JY62_HandleTypeDef *himu) {
#ifndef JY62_MINIMAL
// align successful (reached idle)
	himu->recover_counter++;
#endif
// start the DMA receiving routine
	HAL_UART_Receive_DMA(himu->huart, himu->buffer, 33);
	__HAL_UART_DISABLE_IT(himu->huart, DMA_IT_HT);		// disable half transfer interrupt
}

/*
 * 		Handles the errors.
 * 		@param	himu	jy62 whose port has sent out the error
 * 		@retval	enum IMU_STATUS
 */
void huansic_jy62_error_isr(JY62_HandleTypeDef *himu) {
	if (himu->huart->ErrorCode & HAL_UART_ERROR_DMA) {
		uint32_t TE_FLAG = __HAL_DMA_GET_TE_FLAG_INDEX(himu->huart->hdmarx);
		if (__HAL_DMA_GET_FLAG(himu->huart->hdmarx, TE_FLAG)) {	// if error occurred (double check)
			__HAL_DMA_CLEAR_FLAG(himu->huart->hdmarx, TE_FLAG);
		}
#ifndef JY62_MINIMAL
		himu->dma_hw_error_counter++;
#endif
	} else {
		if (__HAL_UART_GET_FLAG(himu->huart, UART_FLAG_ORE)) {		// it should never happen
			__HAL_UART_CLEAR_OREFLAG(himu->huart);
		}
#ifndef JY62_MINIMAL
		himu->it_hw_error_counter++;
#endif
	}
	HAL_UARTEx_ReceiveToIdle_DMA(himu->huart, himu->buffer, 330);
	__HAL_UART_DISABLE_IT(himu->huart, DMA_IT_HT);		// disable half transfer interrupt
}

/***************	functions used by the library; not visible to users		***************/

static inline void __huansic_jy62_decode_accel(JY62_HandleTypeDef *himu, uint8_t location) {
	int16_t temp;
	uint8_t i;
	for (i = 0; i < 3; i++) {
		temp = himu->buffer[3 + 2 * i + location * 11];
		temp <<= 8;
		temp |= himu->buffer[2 + 2 * i + location * 11];
		himu->accel[i] = (float) temp * 16 * 9.8 / 32768;
	}
	temp = himu->buffer[9 + location * 11];
	temp <<= 8;
	temp |= himu->buffer[8 + location * 11];
	himu->temperature = (float) temp / 340 + 36.53;
	himu->lastUpdated = HAL_GetTick();
#ifndef JY62_MINIMAL
	himu->accel_success_counter++;
#endif
}

static inline void __huansic_jy62_decode_omega(JY62_HandleTypeDef *himu, uint8_t location) {
	int16_t temp;
	uint8_t i;
	for (i = 0; i < 3; i++) {
		temp = himu->buffer[3 + 2 * i + location * 11];
		temp <<= 8;
		temp |= himu->buffer[2 + 2 * i + location * 11];
		himu->omega[i] = (float) temp * 2000 / 32768;
	}
	temp = himu->buffer[9 + location * 11];
	temp <<= 8;
	temp |= himu->buffer[8 + location * 11];
	himu->temperature = (float) temp / 340 + 36.53;
	himu->lastUpdated = HAL_GetTick();
#ifndef JY62_MINIMAL
	himu->omega_success_counter++;
#endif
}

static inline void __huansic_jy62_decode_theta(JY62_HandleTypeDef *himu, uint8_t location) {
	int16_t temp;
	uint8_t i;
	for (i = 0; i < 3; i++) {
		temp = himu->buffer[3 + 2 * i + location * 11];
		temp <<= 8;
		temp |= himu->buffer[2 + 2 * i + location * 11];
		himu->theta[i] = (float) temp * 180 / 32768;
	}
	temp = himu->buffer[9 + location * 11];
	temp <<= 8;
	temp |= himu->buffer[8 + location * 11];
	himu->temperature = (float) temp / 340 + 36.53;
	himu->lastUpdated = HAL_GetTick();
#ifndef JY62_MINIMAL
	himu->theta_success_counter++;
#endif
}

static inline uint8_t __huansic_jy62_checksum(JY62_HandleTypeDef *himu, uint8_t location) {
	uint8_t temp8 = himu->buffer[0 + location * 11];
	temp8 += himu->buffer[1 + location * 11];
	temp8 += himu->buffer[2 + location * 11];
	temp8 += himu->buffer[3 + location * 11];
	temp8 += himu->buffer[4 + location * 11];
	temp8 += himu->buffer[5 + location * 11];
	temp8 += himu->buffer[6 + location * 11];
	temp8 += himu->buffer[7 + location * 11];
	temp8 += himu->buffer[8 + location * 11];
	temp8 += himu->buffer[9 + location * 11];
	return (temp8 == himu->buffer[10 + location * 11]);
}
