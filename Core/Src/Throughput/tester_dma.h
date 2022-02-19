/*
 * tester_dma.h
 *
 *  Created on: Feb 19, 2022
 *      Author: pujak
 */

#ifndef SRC_THROUGHPUT_DMA_H_
#define SRC_THROUGHPUT_DMA_H_

#include "stm32f4xx_hal.h"

/* Public function declarations */
void TS_DMA_Init(void);
void TS_DMA_Transfer(void *DstAddr, void *SrcAddr, uint16_t Size);
void TS_DMA_TansformSize(uint16_t *Size);

void TS_DMA_byte(void);
void TS_DMA_halfword(void);
void TS_DMA_word(void);
void TS_DMA_byte_fifo(void);
void TS_DMA_halfword_fifo(void);
void TS_DMA_word_fifo(void);
void TS_DMA_byte_fifo_inc4(void);
void TS_DMA_byte_fifo_inc8(void);
void TS_DMA_byte_fifo_inc16(void);
void TS_DMA_halfword_fifo_inc4(void);
void TS_DMA_halfword_fifo_inc8(void);
void TS_DMA_word_fifo_inc4(void);

#endif /* SRC_THROUGHPUT_DMA_H_ */
