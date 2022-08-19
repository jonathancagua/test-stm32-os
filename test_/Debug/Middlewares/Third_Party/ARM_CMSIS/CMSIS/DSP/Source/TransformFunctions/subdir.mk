################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.c 

S_UPPER_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.S 

OBJS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.o 

S_UPPER_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.d 

C_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/%.o Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/%.su: ../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/%.c Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_CM4 -D__TARGET_FPU_VFP -D__FPU_PRESENT=1 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F469xx -c -I../Core/Inc -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/%.o: ../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/%.S Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-TransformFunctions

clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-TransformFunctions:
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix8_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.d
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_fast_init_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_f32.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-TransformFunctions

