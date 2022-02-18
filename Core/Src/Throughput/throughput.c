/*
 * throughput.c
 *
 *  Created on: Feb 18, 2022
 *      Author: pujak
 */
#include "./throughput.h"
#include "dma.h"
#include <string.h>

/* Private macros */
#define BUF_SIZE            (1802)
#define ITERATION_CNT       (10000)

/* Private variables */
struct TesterStruct ts_memcpy = { 0 };
struct TesterStruct ts_dma = { 0 };

static uint8_t sram1[BUF_SIZE];
static uint8_t *sram2 = (uint8_t*) 0x2001C000;

static DMA_HandleTypeDef *hdma = &hdma_memtomem_dma2_stream0;
static __IO uint32_t transferError;
static __IO uint32_t transferComplete;

/* Private function declarations */
static uint32_t _average(struct TesterStruct *ts);
static HAL_StatusTypeDef _test(struct TesterStruct *ts, void (*tester)(void));
static inline __always_inline void _tester_memcpy(void);
static inline __always_inline void _tester_dma(void);
static void TransferComplete(DMA_HandleTypeDef *DmaHandle);
static void TransferError(DMA_HandleTypeDef *DmaHandle);

/* Public function definitions */
HAL_StatusTypeDef Throughput_Init(void)
{
  /* register DMA callback */
  HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* enable DWT counter */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  return (HAL_OK);
}

HAL_StatusTypeDef Throughput_Test(void)
{
  uint16_t i;

  for (i = 0; i < ITERATION_CNT; i++) {
    /* Run memcpy() test */
    _test(&ts_memcpy, _tester_memcpy);

    /* Reset DMA flags */
    transferComplete = 0;
    transferError = 0;

    /* Run DMA test */
    _test(&ts_dma, _tester_dma);
  }

  /* averaging */
  _average(&ts_memcpy);
  _average(&ts_dma);

  return (HAL_OK);
}

/* Private function definitions */
static uint32_t _average(struct TesterStruct *ts)
{
  ts->average = ts->duration / ts->count;
  return (ts->average);
}

static HAL_StatusTypeDef _test(struct TesterStruct *ts, void (*tester)(void))
{
  uint32_t tstart, duration;

  /* Validate parameters */
  if (ts == NULL || tester == NULL) {
    return (HAL_ERROR);
  }

  /* Prepare data */
  memset(sram1, 0x55, BUF_SIZE);
  memset(sram2, 0x00, BUF_SIZE);

  /* copy data */
  tstart = DWT->CYCCNT;
  tester();
  duration = DWT->CYCCNT - tstart;

  /* compare data */
  if (memcmp(sram1, sram2, BUF_SIZE) == 0) {
    ts->duration += duration;
    ts->count++;

    return (HAL_OK);
  }
  return (HAL_ERROR);
}

static inline __always_inline void _tester_memcpy(void)
{
  memcpy(sram2, sram1, BUF_SIZE);
}

static inline __always_inline void _tester_dma(void)
{
  HAL_DMA_Start_IT(hdma, (uint32_t) sram1, (uint32_t) sram2, BUF_SIZE);
  while (!transferComplete && !transferError);
}

/* DMA Interrupt Callbacks */
static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
  transferComplete = 1;
}

static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
  transferError = 1;
}
