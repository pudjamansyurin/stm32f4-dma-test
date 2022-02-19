/*
 * tester_dma.c
 *
 *  Created on: Feb 19, 2022
 *      Author: pujak
 */
#include "./tester_dma.h"
#include "dma.h"

/* Private variables */
static DMA_HandleTypeDef *hdma = &hdma_memtomem_dma2_stream0;
static __IO uint32_t transferError;
static __IO uint32_t transferComplete;

/* Private function declarations */
static void _TransferComplete(DMA_HandleTypeDef *hdma);
static void _TransferError(DMA_HandleTypeDef *hdma);

/* Public function definitions */
void TS_DMA_Init(void)
{
  HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_CPLT_CB_ID, _TransferComplete);
  HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, _TransferError);
}

void TS_DMA_Transfer(void *DstAddr, void *SrcAddr, uint16_t Size)
{
  /* Start */
  HAL_DMA_Start_IT(hdma, (uint32_t) SrcAddr, (uint32_t) DstAddr, Size);
  while (!transferComplete && !transferError);
}

void TS_DMA_TansformSize(uint16_t *Size)
{
  /* Reset flags */
  transferComplete = 0;
  transferError = 0;

  /* Convert size */
  if (hdma->Init.MemDataAlignment == DMA_PDATAALIGN_HALFWORD) {
    *Size /= 2;
  } else if (hdma->Init.MemDataAlignment == DMA_PDATAALIGN_WORD) {
    *Size /= 4;
  }
}

void TS_DMA_byte(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_halfword(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_word(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_byte_fifo(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_SINGLE;
  hdma->Init.PeriphBurst = DMA_PBURST_SINGLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_halfword_fifo(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_SINGLE;
  hdma->Init.PeriphBurst = DMA_PBURST_SINGLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_word_fifo(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_SINGLE;
  hdma->Init.PeriphBurst = DMA_PBURST_SINGLE;
  HAL_DMA_Init(hdma);
}

void TS_DMA_byte_fifo_inc4(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC4;
  hdma->Init.PeriphBurst = DMA_MBURST_INC4;
  HAL_DMA_Init(hdma);
}

void TS_DMA_byte_fifo_inc8(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC8;
  hdma->Init.PeriphBurst = DMA_MBURST_INC8;
  HAL_DMA_Init(hdma);
}

void TS_DMA_byte_fifo_inc16(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC16;
  hdma->Init.PeriphBurst = DMA_MBURST_INC16;
  HAL_DMA_Init(hdma);
}

void TS_DMA_halfword_fifo_inc4(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC4;
  hdma->Init.PeriphBurst = DMA_MBURST_INC4;
  HAL_DMA_Init(hdma);
}

void TS_DMA_halfword_fifo_inc8(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC8;
  hdma->Init.PeriphBurst = DMA_MBURST_INC8;
  HAL_DMA_Init(hdma);
}

void TS_DMA_word_fifo_inc4(void)
{
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma->Init.MemBurst = DMA_MBURST_INC4;
  hdma->Init.PeriphBurst = DMA_MBURST_INC4;
  HAL_DMA_Init(hdma);
}

/* DMA Interrupt Callbacks */
static void _TransferComplete(DMA_HandleTypeDef *hdma)
{
  transferComplete = 1;
}

static void _TransferError(DMA_HandleTypeDef *hdma)
{
  transferError = 1;
}
