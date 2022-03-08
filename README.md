# STM32F4 Throughput Test
## memcpy() vs various DMA configuration

To demonstrate DMA FIFO and Burst mode combination performance, a measurement is done using the following system parameters :
- CPU ARM Cortex-M4 (STM32F4)
- System clock (HCLK) is 180MHz.
- Source address SRAM1.
- Destination address SRAM2.
- Data/Frame size 1804 Bytes.
-	DMA FIFO size is 4 words (16 Bytes).
-	Process is iterated over 10000 times to get average result.
-	No other peripherals or task is running, except for copy process.
-	DWT Counter is used as Instruction Cycles monitor.

_Throughput Test Result:_
![DMA Mode Combination](/Assets/dma-poc.jpg)

The experiment also uses combination of transfer direction between SRAM1 and SRAM2, and the result for DMA is varied. But memcpy has stable throughput no matter source and destination address combination. It can be concluded that the SRAM throughput is limited for high transfer, hence the throughput decrease. The bellow DMA measurement is using DMA mode 13 in the table above.

![Transfer Direction Combination](/Assets/transfer-direction.jpg)


_DMA FIFO & BURST Timing Diagram_
![Timing Diagram](/Assets/timing-diagram.png)