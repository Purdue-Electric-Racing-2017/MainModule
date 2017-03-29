################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F407xx -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Inc" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/CMSIS/Include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


