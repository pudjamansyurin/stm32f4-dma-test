/*
 * throughput.c
 *
 *  Created on: Feb 18, 2022
 *      Author: pujak
 */
#include "stm32f4xx-dwt-counter-module/dwt_counter.h"
#include "stm32f4xx-hal-led-module/led.h"
#include "./throughput.h"
#include "./tester_dma.h"
#include <string.h>

/* Private macros */
#define SRAM1_ADDR          ((void*) (0x20000000 + 0x1C000 - 0x4000))
#define SRAM2_ADDR          ((void*) (0x2001C000))
#define BUF_SIZE            (1802)
#define ITERATION_CNT       (10000)

/* External variables */
extern struct Led hled;

/* Private variables */
struct Tester Testers[TEST_CNT] = { 0 };

/* Private function declarations */
static struct Tester* _register_tester(enum TEST_NAME ts_name,
                                       void (*transfer)(void*, void*, uint16_t),
                                       void (*configure)(void));
static HAL_StatusTypeDef _test(struct Tester *ts);
static inline void _average(struct TestCycle *tc);
static inline void _flush(struct TestCycle *tc);
__STATIC_FORCEINLINE void _memcpy_transfer(void *DstAddr,
                                           void *SrcAddr,
                                           uint16_t Size);

/* Public function definitions */
HAL_StatusTypeDef Throughput_Init(void)
{
  TS_DMA_Init();

  /* Initiate testers */
  _register_tester(TEST_MEMCPY, _memcpy_transfer, NULL);

  _register_tester(TEST_DMA_BYTE, TS_DMA_Transfer, TS_DMA_byte);
  _register_tester(TEST_DMA_HALFWORD, TS_DMA_Transfer, TS_DMA_halfword);
  _register_tester(TEST_DMA_WORD, TS_DMA_Transfer, TS_DMA_word);

  _register_tester(TEST_DMA_BYTE_FIFO, TS_DMA_Transfer, TS_DMA_byte_fifo);
  _register_tester(TEST_DMA_HALFWORD_FIFO,
                   TS_DMA_Transfer,
                   TS_DMA_halfword_fifo);
  _register_tester(TEST_DMA_WORD_FIFO, TS_DMA_Transfer, TS_DMA_word_fifo);

  _register_tester(TEST_DMA_BYTE_FIFO_INC4,
                   TS_DMA_Transfer,
                   TS_DMA_byte_fifo_inc4);
  _register_tester(TEST_DMA_BYTE_FIFO_INC8,
                   TS_DMA_Transfer,
                   TS_DMA_byte_fifo_inc8);
  _register_tester(TEST_DMA_BYTE_FIFO_INC16,
                   TS_DMA_Transfer,
                   TS_DMA_byte_fifo_inc16);

  _register_tester(TEST_DMA_HALFWORD_FIFO_INC4,
                   TS_DMA_Transfer,
                   TS_DMA_halfword_fifo_inc4);
  _register_tester(TEST_DMA_HALFWORD_FIFO_INC8,
                   TS_DMA_Transfer,
                   TS_DMA_halfword_fifo_inc8);

  _register_tester(TEST_DMA_WORD_FIFO_INC4,
                   TS_DMA_Transfer,
                   TS_DMA_word_fifo_inc4);

  return (HAL_OK);
}

HAL_StatusTypeDef Throughput_Test(void)
{
  struct Tester *ts;
  uint16_t i, j;

  for (i = 0; i < TEST_CNT; i++) {
    ts = &Testers[i];

    /* Skip if un-registered */
    if (ts->transfer == NULL) {
      continue;
    }

    /* Configure tester */
    if (ts->configure != NULL) {
      ts->configure();
    }

    /* Test routines */
    _flush(&ts->cycle);
    for (j = 0; j < ITERATION_CNT; j++) {
      _test(ts);
    }
    _average(&ts->cycle);

    /* Indicators */
    LED_Write(&hled, 1);
    HAL_Delay(500);
    LED_Write(&hled, 0);
    HAL_Delay(50);
  }

  return (HAL_OK);
}

/* Private function definitions */
static HAL_StatusTypeDef _test(struct Tester *ts)
{
  uint32_t cycles;

  /* Prepare data */
  memset(SRAM1_ADDR, 0x55, BUF_SIZE);
  memset(SRAM2_ADDR, 0x00, BUF_SIZE);

  TS_DMA_ResetFlags();

  /* Transfer data */
  DWT_Init();
  ts->transfer(SRAM2_ADDR, SRAM1_ADDR, BUF_SIZE);
  cycles = DWT_GetCounter();
  DWT_DeInit();

  /* Compare data */
  if (memcmp(SRAM1_ADDR, SRAM2_ADDR, BUF_SIZE) == 0) {
    ts->cycle.total += cycles;
    ts->cycle.count++;

    return (HAL_OK);
  }
  return (HAL_ERROR);
}

static inline void _average(struct TestCycle *tc)
{
  tc->average = tc->total / tc->count;
}

static inline void _flush(struct TestCycle *tc)
{
  tc->count = 0;
  tc->total = 0;
  tc->average = 0;
}

static struct Tester* _register_tester(enum TEST_NAME ts_name,
                                       void (*transfer)(void*, void*, uint16_t),
                                       void (*configure)(void))
{
  struct Tester *ts = &Testers[ts_name];

  _flush(&ts->cycle);
  ts->transfer = transfer;
  ts->configure = configure;

  return (ts);
}

__STATIC_FORCEINLINE void _memcpy_transfer(void *DstAddr,
                                           void *SrcAddr,
                                           uint16_t Size)
{
  memcpy(DstAddr, SrcAddr, Size);
}
