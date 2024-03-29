################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.c 

OBJS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.o 

C_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/%.o Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/%.su: ../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/%.c Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -D__TARGET_FPU_VFP -D__FPU_PRESENT=1 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FastMathFunctions

clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FastMathFunctions:
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FastMathFunctions

