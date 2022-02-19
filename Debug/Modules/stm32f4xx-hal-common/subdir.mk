################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/stm32f4xx-hal-common/common.c 

OBJS += \
./Modules/stm32f4xx-hal-common/common.o 

C_DEPS += \
./Modules/stm32f4xx-hal-common/common.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/stm32f4xx-hal-common/%.o: ../Modules/stm32f4xx-hal-common/%.c Modules/stm32f4xx-hal-common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src -I../Modules -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Modules-2f-stm32f4xx-2d-hal-2d-common

clean-Modules-2f-stm32f4xx-2d-hal-2d-common:
	-$(RM) ./Modules/stm32f4xx-hal-common/common.d ./Modules/stm32f4xx-hal-common/common.o

.PHONY: clean-Modules-2f-stm32f4xx-2d-hal-2d-common

