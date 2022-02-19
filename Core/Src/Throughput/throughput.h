/*
 * throughput.h
 *
 *  Created on: Feb 18, 2022
 *      Author: pujak
 */

#ifndef SRC_THROUGHPUT_THROUGHPUT_H_
#define SRC_THROUGHPUT_THROUGHPUT_H_

#include "stm32f4xx_hal.h"

/* Public enums */
enum TEST_NAME {
  TEST_MEMCPY,
  TEST_DMA_BYTE,
  TEST_DMA_HALFWORD,
  TEST_DMA_WORD,
  TEST_DMA_BYTE_FIFO,
  TEST_DMA_HALFWORD_FIFO,
  TEST_DMA_WORD_FIFO,
  TEST_DMA_BYTE_FIFO_INC4,
  TEST_DMA_BYTE_FIFO_INC8,
  TEST_DMA_BYTE_FIFO_INC16,
  TEST_DMA_HALFWORD_FIFO_INC4,
  TEST_DMA_HALFWORD_FIFO_INC8,
  TEST_DMA_WORD_FIFO_INC4,
  TEST_CNT,
};

/* Public types */
struct TestCycle {
  uint64_t total;
  uint16_t count;
  uint32_t average;
};

struct Tester {
  struct TestCycle cycle;
  void (*configure)(void);
  void (*transfer)(void*, void*, uint16_t);
};

/* Public function declarations */
HAL_StatusTypeDef Throughput_Init(void);
HAL_StatusTypeDef Throughput_Test(void);

#endif /* SRC_THROUGHPUT_THROUGHPUT_H_ */
