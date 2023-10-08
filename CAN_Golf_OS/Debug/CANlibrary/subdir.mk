################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CANlibrary/CAN_Init.c \
../CANlibrary/CAN_OSI.c \
../CANlibrary/CAN_Process.c 

OBJS += \
./CANlibrary/CAN_Init.o \
./CANlibrary/CAN_OSI.o \
./CANlibrary/CAN_Process.o 

C_DEPS += \
./CANlibrary/CAN_Init.d \
./CANlibrary/CAN_OSI.d \
./CANlibrary/CAN_Process.d 


# Each subdirectory must supply rules for building sources it contributes
CANlibrary/%.o CANlibrary/%.su CANlibrary/%.cyclo: ../CANlibrary/%.c CANlibrary/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../CANlibrary -I../CRC -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CANlibrary

clean-CANlibrary:
	-$(RM) ./CANlibrary/CAN_Init.cyclo ./CANlibrary/CAN_Init.d ./CANlibrary/CAN_Init.o ./CANlibrary/CAN_Init.su ./CANlibrary/CAN_OSI.cyclo ./CANlibrary/CAN_OSI.d ./CANlibrary/CAN_OSI.o ./CANlibrary/CAN_OSI.su ./CANlibrary/CAN_Process.cyclo ./CANlibrary/CAN_Process.d ./CANlibrary/CAN_Process.o ./CANlibrary/CAN_Process.su

.PHONY: clean-CANlibrary

