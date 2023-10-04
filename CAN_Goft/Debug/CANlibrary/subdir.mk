################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CANlibrary/CAN_Flag.c \
../CANlibrary/canframe.c 

OBJS += \
./CANlibrary/CAN_Flag.o \
./CANlibrary/canframe.o 

C_DEPS += \
./CANlibrary/CAN_Flag.d \
./CANlibrary/canframe.d 


# Each subdirectory must supply rules for building sources it contributes
CANlibrary/%.o CANlibrary/%.su CANlibrary/%.cyclo: ../CANlibrary/%.c CANlibrary/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../CRC -I../CANlibrary -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CANlibrary

clean-CANlibrary:
	-$(RM) ./CANlibrary/CAN_Flag.cyclo ./CANlibrary/CAN_Flag.d ./CANlibrary/CAN_Flag.o ./CANlibrary/CAN_Flag.su ./CANlibrary/canframe.cyclo ./CANlibrary/canframe.d ./CANlibrary/canframe.o ./CANlibrary/canframe.su

.PHONY: clean-CANlibrary

