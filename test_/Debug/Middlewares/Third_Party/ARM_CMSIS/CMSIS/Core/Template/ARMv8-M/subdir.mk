################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.c 

OBJS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.o 

C_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/%.o Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/%.su: ../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/%.c Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_CM4 -D__TARGET_FPU_VFP -D__FPU_PRESENT=1 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F469xx -c -I../Core/Inc -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M

clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M:
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/main_s.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Template/ARMv8-M/tz_context.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M

