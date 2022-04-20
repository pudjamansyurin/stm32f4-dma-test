/*
 * throughput.c
 *
 *  Created on: Feb 18, 2022
 *      Author: pujak
 */
#include "stm32f4xx-hal-dwt-counter/dwt_counter.h"
#include "stm32f4xx-hal-led/led.h"
#include "./throughput.h"
#include "./tester_dma.h"
#include <string.h>

/* Private macros */
#define BUF_SIZE            (1804)
#define ITERATION_CNT       (10000)
#define SRAM1_ADDR          ((void*) (0x2001C000))
#define SRAM2_ADDR          ((void*) (0x2001C000 + BUF_SIZE))

/* External variables */
extern struct Led hled;

/* Private variables */
struct Tester Testers[TEST_CNT] = { 0 };
//uint8_t SRAM1_ADDR[BUF_SIZE];
//uint8_t SRAM2_ADDR[BUF_SIZE];

/* Private function declarations */
static void _register(enum TEST_NAME ts_name,
                      void (*transfer)(void*, void*, uint16_t),
                      void (*transform)(uint16_t*),
                      void (*configure)(void));
static void _register_dma(enum TEST_NAME ts_name, void (*configure)(void));
static HAL_StatusTypeDef _test(struct Tester *ts);
static inline void _calculate(struct Tester *ts);
static inline void _flush(struct Tester *ts);
__STATIC_FORCEINLINE void _memcpy_transfer(void *DstAddr,
                                           void *SrcAddr,
                                           uint16_t Size);

/* Public function definitions */
HAL_StatusTypeDef Throughput_Init(void)
{
  TS_DMA_Init();
  memset(SRAM1_ADDR, 0x77, BUF_SIZE);

  /* Initiate testers */
  _register(TEST_MEMCPY, _memcpy_transfer, NULL, NULL);

  _register_dma(TEST_DMA_BYTE, TS_DMA_byte);
  _register_dma(TEST_DMA_HALFWORD, TS_DMA_halfword);
  _register_dma(TEST_DMA_WORD, TS_DMA_word);

  _register_dma(TEST_DMA_BYTE_FIFO, TS_DMA_byte_fifo);
  _register_dma(TEST_DMA_HALFWORD_FIFO, TS_DMA_halfword_fifo);
  _register_dma(TEST_DMA_WORD_FIFO, TS_DMA_word_fifo);

  _register_dma(TEST_DMA_BYTE_FIFO_INC4, TS_DMA_byte_fifo_inc4);
  _register_dma(TEST_DMA_BYTE_FIFO_INC8, TS_DMA_byte_fifo_inc8);
  _register_dma(TEST_DMA_BYTE_FIFO_INC16, TS_DMA_byte_fifo_inc16);

  _register_dma(TEST_DMA_HALFWORD_FIFO_INC4, TS_DMA_halfword_fifo_inc4);
  _register_dma(TEST_DMA_HALFWORD_FIFO_INC8, TS_DMA_halfword_fifo_inc8);

  _register_dma(TEST_DMA_WORD_FIFO_INC4, TS_DMA_word_fifo_inc4);

  return (HAL_OK);
}

HAL_StatusTypeDef Throughput_Test(void)
{
  HAL_StatusTypeDef status;
  struct Tester *ts;
  uint8_t i;
  uint16_t j;

  for (i = 0; i < TEST_CNT; i++)
  {
    ts = &Testers[i];

    /* Skip if un-registered */
    if (ts->transfer == NULL)
    {
      continue;
    }

    /* Configure tester */
    if (ts->configure != NULL)
    {
      ts->configure();
    }

    /* Test routines */
    _flush(ts);
    for (j = 0; j < ITERATION_CNT; j++)
    {
      status = _test(ts);
    }

    /* Summarize result */
    if (status == HAL_OK)
    {
      _calculate(ts);
    }

    /* Indicators */
    LED_Blink(&hled, 500, 50);
  }

  return (HAL_OK);
}

/* Private function definitions */
static HAL_StatusTypeDef _test(struct Tester *ts)
{
  uint16_t size = BUF_SIZE;
  uint32_t cycles;

  /* Prepare data */
  memset(SRAM2_ADDR, 0x00, BUF_SIZE);

  /* Transform size */
  if (ts->transform != NULL)
  {
    ts->transform(&size);
  }

  /* Transfer data */
  DWT_Start();
  ts->transfer(SRAM2_ADDR, SRAM1_ADDR, size);
  cycles = DWT_GetCycle();
  DWT_Stop();

  /* Compare data */
  if (memcmp(SRAM1_ADDR, SRAM2_ADDR, BUF_SIZE) == 0)
  {
    ts->cycle_total += cycles;
    ts->iteration++;

    return (HAL_OK);
  }
  return (HAL_ERROR);
}

static inline void _calculate(struct Tester *ts)
{
  uint32_t hclock_Mhz = HAL_RCC_GetHCLKFreq() / 1000000;

  ts->cycles = ts->cycle_total / ts->iteration;
  ts->duration_us = (float) ts->cycles / hclock_Mhz;
  ts->kfps = 1000.0f / ts->duration_us;
  ts->byte_cycles = (float) ts->cycles / BUF_SIZE;
  ts->byte_duration_ns = ts->duration_us * 1000.0f / BUF_SIZE;
  ts->MBps = 1000000000.0f / ts->byte_duration_ns / 1024.0f / 1024.0f;
}

static inline void _flush(struct Tester *ts)
{
  ts->iteration = 0;
  ts->cycle_total = 0;
  ts->cycles = 0;
  ts->duration_us = 0.0f;
  ts->kfps = 0.0f;
  ts->byte_cycles = 0.0f;
  ts->byte_duration_ns = 0.0f;
  ts->MBps = 0.0f;
}

static void _register(enum TEST_NAME ts_name,
                      void (*transfer)(void*, void*, uint16_t),
                      void (*transform)(uint16_t*),
                      void (*configure)(void))
{
  struct Tester *ts = &Testers[ts_name];

  _flush(ts);
  ts->transfer = transfer;
  ts->transform = transform;
  ts->configure = configure;
}

static void _register_dma(enum TEST_NAME ts_name, void (*configure)(void))
{
  _register(ts_name, TS_DMA_Transfer, TS_DMA_TansformSize, configure);
}

__STATIC_FORCEINLINE void _memcpy_transfer(void *DstAddr,
                                           void *SrcAddr,
                                           uint16_t Size)
{
  memcpy(DstAddr, SrcAddr, Size);
}
