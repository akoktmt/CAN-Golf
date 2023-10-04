################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uart/usart1.c 

OBJS += \
./uart/usart1.o 

C_DEPS += \
./uart/usart1.d 


# Each subdirectory must supply rules for building sources it contributes
uart/%.o uart/%.su uart/%.cyclo: ../uart/%.c uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../CRC -I../CANlibrary -I../uart -I"D:/CAN_Workspace/CAN_Goft/uart" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-uart

clean-uart:
	-$(RM) ./uart/usart1.cyclo ./uart/usart1.d ./uart/usart1.o ./uart/usart1.su

.PHONY: clean-uart

