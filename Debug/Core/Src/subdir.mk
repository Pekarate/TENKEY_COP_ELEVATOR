################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CanBus.c \
../Core/Src/Dwin.c \
../Core/Src/Dwin_App.c \
../Core/Src/Init.c \
../Core/Src/MyDrict.c \
../Core/Src/OLd_Main.c \
../Core/Src/SubProg.c \
../Core/Src/Uart.c \
../Core/Src/Version.c \
../Core/Src/flash.c \
../Core/Src/main.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/CanBus.o \
./Core/Src/Dwin.o \
./Core/Src/Dwin_App.o \
./Core/Src/Init.o \
./Core/Src/MyDrict.o \
./Core/Src/OLd_Main.o \
./Core/Src/SubProg.o \
./Core/Src/Uart.o \
./Core/Src/Version.o \
./Core/Src/flash.o \
./Core/Src/main.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/CanBus.d \
./Core/Src/Dwin.d \
./Core/Src/Dwin_App.d \
./Core/Src/Init.d \
./Core/Src/MyDrict.d \
./Core/Src/OLd_Main.d \
./Core/Src/SubProg.d \
./Core/Src/Uart.d \
./Core/Src/Version.d \
./Core/Src/flash.d \
./Core/Src/main.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CanBus.d ./Core/Src/CanBus.o ./Core/Src/Dwin.d ./Core/Src/Dwin.o ./Core/Src/Dwin_App.d ./Core/Src/Dwin_App.o ./Core/Src/Init.d ./Core/Src/Init.o ./Core/Src/MyDrict.d ./Core/Src/MyDrict.o ./Core/Src/OLd_Main.d ./Core/Src/OLd_Main.o ./Core/Src/SubProg.d ./Core/Src/SubProg.o ./Core/Src/Uart.d ./Core/Src/Uart.o ./Core/Src/Version.d ./Core/Src/Version.o ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o

.PHONY: clean-Core-2f-Src

