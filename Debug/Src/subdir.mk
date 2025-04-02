################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/M20071_GPS.c \
../Src/lora.c \
../Src/lsm6dsox_reg.c \
../Src/main.c \
../Src/stm32wlxx_hal_msp.c \
../Src/syscalls.c \
../Src/sysmem.c 

C_DEPS += \
./Src/M20071_GPS.d \
./Src/lora.d \
./Src/lsm6dsox_reg.d \
./Src/main.d \
./Src/stm32wlxx_hal_msp.d \
./Src/syscalls.d \
./Src/sysmem.d 

OBJS += \
./Src/M20071_GPS.o \
./Src/lora.o \
./Src/lsm6dsox_reg.o \
./Src/main.o \
./Src/stm32wlxx_hal_msp.o \
./Src/syscalls.o \
./Src/sysmem.o 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32WLSINGLE -DSTM32WL -DSTM32WLE5JCIx -DSTM32 -DSTM32WLE5xx -c -I../Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Src -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/M20071_GPS.cyclo ./Src/M20071_GPS.d ./Src/M20071_GPS.o ./Src/M20071_GPS.su ./Src/lora.cyclo ./Src/lora.d ./Src/lora.o ./Src/lora.su ./Src/lsm6dsox_reg.cyclo ./Src/lsm6dsox_reg.d ./Src/lsm6dsox_reg.o ./Src/lsm6dsox_reg.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32wlxx_hal_msp.cyclo ./Src/stm32wlxx_hal_msp.d ./Src/stm32wlxx_hal_msp.o ./Src/stm32wlxx_hal_msp.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

