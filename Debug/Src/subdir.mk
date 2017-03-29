################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BMS.c \
../Src/CANProcess.c \
../Src/Motor_Controller.c \
../Src/PedalBox.c \
../Src/UserTasks.c \
../Src/WheelModule.c \
../Src/car.c \
../Src/freertos.c \
../Src/main.c \
../Src/main_module_tasks.c \
../Src/motor_controller_functions.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_TIM.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/BMS.o \
./Src/CANProcess.o \
./Src/Motor_Controller.o \
./Src/PedalBox.o \
./Src/UserTasks.o \
./Src/WheelModule.o \
./Src/car.o \
./Src/freertos.o \
./Src/main.o \
./Src/main_module_tasks.o \
./Src/motor_controller_functions.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_TIM.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/BMS.d \
./Src/CANProcess.d \
./Src/Motor_Controller.d \
./Src/PedalBox.d \
./Src/UserTasks.d \
./Src/WheelModule.d \
./Src/car.d \
./Src/freertos.d \
./Src/main.d \
./Src/main_module_tasks.d \
./Src/motor_controller_functions.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_TIM.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F407xx -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Inc" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Drivers/CMSIS/Include" -I"/home/ben/Data/PER/STM32CubeMX/MainModule/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


