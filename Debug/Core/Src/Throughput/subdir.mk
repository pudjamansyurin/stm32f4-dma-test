################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Throughput/throughput.c 

OBJS += \
./Core/Src/Throughput/throughput.o 

C_DEPS += \
./Core/Src/Throughput/throughput.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Throughput/%.o: ../Core/Src/Throughput/%.c Core/Src/Throughput/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Throughput

clean-Core-2f-Src-2f-Throughput:
	-$(RM) ./Core/Src/Throughput/throughput.d ./Core/Src/Throughput/throughput.o

.PHONY: clean-Core-2f-Src-2f-Throughput

