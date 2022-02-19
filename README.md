# STM32F4 Throughtput Test
## memcpy() vs various DMA configuratinon

To demonstrate DMA FIFO and Burst mode combination performance based on the table above, a measurement is done using the following system parameters :
- CPU ARM Cortex-M4 (STM32F4)
- System clock (HCLK) is 180MHz.
- Source address SRAM1.
- Destination address SRAM2.
- Data size 1802 Bytes.
-	DMA FIFO size is 4 words (16 Bytes).
-	Process is iterated over 10000 times to get average result.
-	No other peripherals or task is running, except for copy process.
-	DWT Counter is used as Instruction Cycles monitor.

Results:
![Throughput Test Result](/dma-poc.jpg)