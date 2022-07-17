################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/led.c \
../Src/main.c \
../Src/os_kernel.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timebase.c \
../Src/uart.c 

S_UPPER_SRCS += \
../Src/SysTick_Handler.S 

OBJS += \
./Src/SysTick_Handler.o \
./Src/led.o \
./Src/main.o \
./Src/os_kernel.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timebase.o \
./Src/uart.o 

S_UPPER_DEPS += \
./Src/SysTick_Handler.d 

C_DEPS += \
./Src/led.d \
./Src/main.d \
./Src/os_kernel.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timebase.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.S Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L475xx -c -I../Inc -I"/Users/jonathancagua/STM32/test-stm32-os/5_test_os/chip_headers/CMSIS/Include" -I"/Users/jonathancagua/STM32/test-stm32-os/5_test_os/chip_headers/CMSIS/Device/ST/STM32L4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/SysTick_Handler.d ./Src/SysTick_Handler.o ./Src/led.d ./Src/led.o ./Src/led.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/os_kernel.d ./Src/os_kernel.o ./Src/os_kernel.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

