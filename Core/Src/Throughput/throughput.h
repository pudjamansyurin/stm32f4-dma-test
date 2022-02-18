/*
 * throughput.h
 *
 *  Created on: Feb 18, 2022
 *      Author: pujak
 */

#ifndef SRC_THROUGHPUT_THROUGHPUT_H_
#define SRC_THROUGHPUT_THROUGHPUT_H_

#include "stm32f4xx_hal.h"

/* Public types */
struct TesterStruct {
  uint32_t duration;
  uint16_t count;
  uint32_t average;
};

/* Public function declarations */
HAL_StatusTypeDef Throughput_Init(void);
HAL_StatusTypeDef Throughput_Test(void);

#endif /* SRC_THROUGHPUT_THROUGHPUT_H_ */
