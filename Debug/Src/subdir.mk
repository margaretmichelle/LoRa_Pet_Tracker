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
../Src/stm32wlxx_hal.c \
../Src/stm32wlxx_hal_adc.c \
../Src/stm32wlxx_hal_adc_ex.c \
../Src/stm32wlxx_hal_comp.c \
../Src/stm32wlxx_hal_cortex.c \
../Src/stm32wlxx_hal_crc.c \
../Src/stm32wlxx_hal_crc_ex.c \
../Src/stm32wlxx_hal_cryp.c \
../Src/stm32wlxx_hal_cryp_ex.c \
../Src/stm32wlxx_hal_dac.c \
../Src/stm32wlxx_hal_dac_ex.c \
../Src/stm32wlxx_hal_dma.c \
../Src/stm32wlxx_hal_dma_ex.c \
../Src/stm32wlxx_hal_exti.c \
../Src/stm32wlxx_hal_flash.c \
../Src/stm32wlxx_hal_flash_ex.c \
../Src/stm32wlxx_hal_gpio.c \
../Src/stm32wlxx_hal_gtzc.c \
../Src/stm32wlxx_hal_hsem.c \
../Src/stm32wlxx_hal_i2c.c \
../Src/stm32wlxx_hal_i2c_ex.c \
../Src/stm32wlxx_hal_i2s.c \
../Src/stm32wlxx_hal_ipcc.c \
../Src/stm32wlxx_hal_irda.c \
../Src/stm32wlxx_hal_iwdg.c \
../Src/stm32wlxx_hal_lptim.c \
../Src/stm32wlxx_hal_msp.c \
../Src/stm32wlxx_hal_pka.c \
../Src/stm32wlxx_hal_pwr.c \
../Src/stm32wlxx_hal_pwr_ex.c \
../Src/stm32wlxx_hal_rcc.c \
../Src/stm32wlxx_hal_rcc_ex.c \
../Src/stm32wlxx_hal_rng.c \
../Src/stm32wlxx_hal_rng_ex.c \
../Src/stm32wlxx_hal_rtc.c \
../Src/stm32wlxx_hal_rtc_ex.c \
../Src/stm32wlxx_hal_smartcard.c \
../Src/stm32wlxx_hal_smartcard_ex.c \
../Src/stm32wlxx_hal_smbus.c \
../Src/stm32wlxx_hal_smbus_ex.c \
../Src/stm32wlxx_hal_spi.c \
../Src/stm32wlxx_hal_spi_ex.c \
../Src/stm32wlxx_hal_subghz.c \
../Src/stm32wlxx_hal_tim.c \
../Src/stm32wlxx_hal_tim_ex.c \
../Src/stm32wlxx_hal_uart.c \
../Src/stm32wlxx_hal_uart_ex.c \
../Src/stm32wlxx_hal_usart.c \
../Src/stm32wlxx_hal_usart_ex.c \
../Src/stm32wlxx_hal_wwdg.c \
../Src/stm32wlxx_ll_adc.c \
../Src/stm32wlxx_ll_comp.c \
../Src/stm32wlxx_ll_crc.c \
../Src/stm32wlxx_ll_dac.c \
../Src/stm32wlxx_ll_dma.c \
../Src/stm32wlxx_ll_exti.c \
../Src/stm32wlxx_ll_gpio.c \
../Src/stm32wlxx_ll_i2c.c \
../Src/stm32wlxx_ll_lptim.c \
../Src/stm32wlxx_ll_lpuart.c \
../Src/stm32wlxx_ll_pka.c \
../Src/stm32wlxx_ll_pwr.c \
../Src/stm32wlxx_ll_rcc.c \
../Src/stm32wlxx_ll_rng.c \
../Src/stm32wlxx_ll_rtc.c \
../Src/stm32wlxx_ll_spi.c \
../Src/stm32wlxx_ll_tim.c \
../Src/stm32wlxx_ll_usart.c \
../Src/stm32wlxx_ll_utils.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32wlxx.c 

C_DEPS += \
./Src/M20071_GPS.d \
./Src/lora.d \
./Src/lsm6dsox_reg.d \
./Src/main.d \
./Src/stm32wlxx_hal.d \
./Src/stm32wlxx_hal_adc.d \
./Src/stm32wlxx_hal_adc_ex.d \
./Src/stm32wlxx_hal_comp.d \
./Src/stm32wlxx_hal_cortex.d \
./Src/stm32wlxx_hal_crc.d \
./Src/stm32wlxx_hal_crc_ex.d \
./Src/stm32wlxx_hal_cryp.d \
./Src/stm32wlxx_hal_cryp_ex.d \
./Src/stm32wlxx_hal_dac.d \
./Src/stm32wlxx_hal_dac_ex.d \
./Src/stm32wlxx_hal_dma.d \
./Src/stm32wlxx_hal_dma_ex.d \
./Src/stm32wlxx_hal_exti.d \
./Src/stm32wlxx_hal_flash.d \
./Src/stm32wlxx_hal_flash_ex.d \
./Src/stm32wlxx_hal_gpio.d \
./Src/stm32wlxx_hal_gtzc.d \
./Src/stm32wlxx_hal_hsem.d \
./Src/stm32wlxx_hal_i2c.d \
./Src/stm32wlxx_hal_i2c_ex.d \
./Src/stm32wlxx_hal_i2s.d \
./Src/stm32wlxx_hal_ipcc.d \
./Src/stm32wlxx_hal_irda.d \
./Src/stm32wlxx_hal_iwdg.d \
./Src/stm32wlxx_hal_lptim.d \
./Src/stm32wlxx_hal_msp.d \
./Src/stm32wlxx_hal_pka.d \
./Src/stm32wlxx_hal_pwr.d \
./Src/stm32wlxx_hal_pwr_ex.d \
./Src/stm32wlxx_hal_rcc.d \
./Src/stm32wlxx_hal_rcc_ex.d \
./Src/stm32wlxx_hal_rng.d \
./Src/stm32wlxx_hal_rng_ex.d \
./Src/stm32wlxx_hal_rtc.d \
./Src/stm32wlxx_hal_rtc_ex.d \
./Src/stm32wlxx_hal_smartcard.d \
./Src/stm32wlxx_hal_smartcard_ex.d \
./Src/stm32wlxx_hal_smbus.d \
./Src/stm32wlxx_hal_smbus_ex.d \
./Src/stm32wlxx_hal_spi.d \
./Src/stm32wlxx_hal_spi_ex.d \
./Src/stm32wlxx_hal_subghz.d \
./Src/stm32wlxx_hal_tim.d \
./Src/stm32wlxx_hal_tim_ex.d \
./Src/stm32wlxx_hal_uart.d \
./Src/stm32wlxx_hal_uart_ex.d \
./Src/stm32wlxx_hal_usart.d \
./Src/stm32wlxx_hal_usart_ex.d \
./Src/stm32wlxx_hal_wwdg.d \
./Src/stm32wlxx_ll_adc.d \
./Src/stm32wlxx_ll_comp.d \
./Src/stm32wlxx_ll_crc.d \
./Src/stm32wlxx_ll_dac.d \
./Src/stm32wlxx_ll_dma.d \
./Src/stm32wlxx_ll_exti.d \
./Src/stm32wlxx_ll_gpio.d \
./Src/stm32wlxx_ll_i2c.d \
./Src/stm32wlxx_ll_lptim.d \
./Src/stm32wlxx_ll_lpuart.d \
./Src/stm32wlxx_ll_pka.d \
./Src/stm32wlxx_ll_pwr.d \
./Src/stm32wlxx_ll_rcc.d \
./Src/stm32wlxx_ll_rng.d \
./Src/stm32wlxx_ll_rtc.d \
./Src/stm32wlxx_ll_spi.d \
./Src/stm32wlxx_ll_tim.d \
./Src/stm32wlxx_ll_usart.d \
./Src/stm32wlxx_ll_utils.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32wlxx.d 

OBJS += \
./Src/M20071_GPS.o \
./Src/lora.o \
./Src/lsm6dsox_reg.o \
./Src/main.o \
./Src/stm32wlxx_hal.o \
./Src/stm32wlxx_hal_adc.o \
./Src/stm32wlxx_hal_adc_ex.o \
./Src/stm32wlxx_hal_comp.o \
./Src/stm32wlxx_hal_cortex.o \
./Src/stm32wlxx_hal_crc.o \
./Src/stm32wlxx_hal_crc_ex.o \
./Src/stm32wlxx_hal_cryp.o \
./Src/stm32wlxx_hal_cryp_ex.o \
./Src/stm32wlxx_hal_dac.o \
./Src/stm32wlxx_hal_dac_ex.o \
./Src/stm32wlxx_hal_dma.o \
./Src/stm32wlxx_hal_dma_ex.o \
./Src/stm32wlxx_hal_exti.o \
./Src/stm32wlxx_hal_flash.o \
./Src/stm32wlxx_hal_flash_ex.o \
./Src/stm32wlxx_hal_gpio.o \
./Src/stm32wlxx_hal_gtzc.o \
./Src/stm32wlxx_hal_hsem.o \
./Src/stm32wlxx_hal_i2c.o \
./Src/stm32wlxx_hal_i2c_ex.o \
./Src/stm32wlxx_hal_i2s.o \
./Src/stm32wlxx_hal_ipcc.o \
./Src/stm32wlxx_hal_irda.o \
./Src/stm32wlxx_hal_iwdg.o \
./Src/stm32wlxx_hal_lptim.o \
./Src/stm32wlxx_hal_msp.o \
./Src/stm32wlxx_hal_pka.o \
./Src/stm32wlxx_hal_pwr.o \
./Src/stm32wlxx_hal_pwr_ex.o \
./Src/stm32wlxx_hal_rcc.o \
./Src/stm32wlxx_hal_rcc_ex.o \
./Src/stm32wlxx_hal_rng.o \
./Src/stm32wlxx_hal_rng_ex.o \
./Src/stm32wlxx_hal_rtc.o \
./Src/stm32wlxx_hal_rtc_ex.o \
./Src/stm32wlxx_hal_smartcard.o \
./Src/stm32wlxx_hal_smartcard_ex.o \
./Src/stm32wlxx_hal_smbus.o \
./Src/stm32wlxx_hal_smbus_ex.o \
./Src/stm32wlxx_hal_spi.o \
./Src/stm32wlxx_hal_spi_ex.o \
./Src/stm32wlxx_hal_subghz.o \
./Src/stm32wlxx_hal_tim.o \
./Src/stm32wlxx_hal_tim_ex.o \
./Src/stm32wlxx_hal_uart.o \
./Src/stm32wlxx_hal_uart_ex.o \
./Src/stm32wlxx_hal_usart.o \
./Src/stm32wlxx_hal_usart_ex.o \
./Src/stm32wlxx_hal_wwdg.o \
./Src/stm32wlxx_ll_adc.o \
./Src/stm32wlxx_ll_comp.o \
./Src/stm32wlxx_ll_crc.o \
./Src/stm32wlxx_ll_dac.o \
./Src/stm32wlxx_ll_dma.o \
./Src/stm32wlxx_ll_exti.o \
./Src/stm32wlxx_ll_gpio.o \
./Src/stm32wlxx_ll_i2c.o \
./Src/stm32wlxx_ll_lptim.o \
./Src/stm32wlxx_ll_lpuart.o \
./Src/stm32wlxx_ll_pka.o \
./Src/stm32wlxx_ll_pwr.o \
./Src/stm32wlxx_ll_rcc.o \
./Src/stm32wlxx_ll_rng.o \
./Src/stm32wlxx_ll_rtc.o \
./Src/stm32wlxx_ll_spi.o \
./Src/stm32wlxx_ll_tim.o \
./Src/stm32wlxx_ll_usart.o \
./Src/stm32wlxx_ll_utils.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32wlxx.o 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DSTM32WLSINGLE -DSTM32WL -DSTM32WLE5JCIx -DSTM32 -c -I../Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/M20071_GPS.cyclo ./Src/M20071_GPS.d ./Src/M20071_GPS.o ./Src/M20071_GPS.su ./Src/lora.cyclo ./Src/lora.d ./Src/lora.o ./Src/lora.su ./Src/lsm6dsox_reg.cyclo ./Src/lsm6dsox_reg.d ./Src/lsm6dsox_reg.o ./Src/lsm6dsox_reg.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32wlxx_hal.cyclo ./Src/stm32wlxx_hal.d ./Src/stm32wlxx_hal.o ./Src/stm32wlxx_hal.su ./Src/stm32wlxx_hal_adc.cyclo ./Src/stm32wlxx_hal_adc.d ./Src/stm32wlxx_hal_adc.o ./Src/stm32wlxx_hal_adc.su ./Src/stm32wlxx_hal_adc_ex.cyclo ./Src/stm32wlxx_hal_adc_ex.d ./Src/stm32wlxx_hal_adc_ex.o ./Src/stm32wlxx_hal_adc_ex.su ./Src/stm32wlxx_hal_comp.cyclo ./Src/stm32wlxx_hal_comp.d ./Src/stm32wlxx_hal_comp.o ./Src/stm32wlxx_hal_comp.su ./Src/stm32wlxx_hal_cortex.cyclo ./Src/stm32wlxx_hal_cortex.d ./Src/stm32wlxx_hal_cortex.o ./Src/stm32wlxx_hal_cortex.su ./Src/stm32wlxx_hal_crc.cyclo ./Src/stm32wlxx_hal_crc.d ./Src/stm32wlxx_hal_crc.o ./Src/stm32wlxx_hal_crc.su ./Src/stm32wlxx_hal_crc_ex.cyclo ./Src/stm32wlxx_hal_crc_ex.d ./Src/stm32wlxx_hal_crc_ex.o ./Src/stm32wlxx_hal_crc_ex.su ./Src/stm32wlxx_hal_cryp.cyclo ./Src/stm32wlxx_hal_cryp.d ./Src/stm32wlxx_hal_cryp.o ./Src/stm32wlxx_hal_cryp.su ./Src/stm32wlxx_hal_cryp_ex.cyclo ./Src/stm32wlxx_hal_cryp_ex.d ./Src/stm32wlxx_hal_cryp_ex.o ./Src/stm32wlxx_hal_cryp_ex.su ./Src/stm32wlxx_hal_dac.cyclo ./Src/stm32wlxx_hal_dac.d ./Src/stm32wlxx_hal_dac.o ./Src/stm32wlxx_hal_dac.su ./Src/stm32wlxx_hal_dac_ex.cyclo ./Src/stm32wlxx_hal_dac_ex.d ./Src/stm32wlxx_hal_dac_ex.o ./Src/stm32wlxx_hal_dac_ex.su ./Src/stm32wlxx_hal_dma.cyclo ./Src/stm32wlxx_hal_dma.d ./Src/stm32wlxx_hal_dma.o ./Src/stm32wlxx_hal_dma.su ./Src/stm32wlxx_hal_dma_ex.cyclo ./Src/stm32wlxx_hal_dma_ex.d ./Src/stm32wlxx_hal_dma_ex.o ./Src/stm32wlxx_hal_dma_ex.su ./Src/stm32wlxx_hal_exti.cyclo ./Src/stm32wlxx_hal_exti.d ./Src/stm32wlxx_hal_exti.o ./Src/stm32wlxx_hal_exti.su ./Src/stm32wlxx_hal_flash.cyclo ./Src/stm32wlxx_hal_flash.d ./Src/stm32wlxx_hal_flash.o ./Src/stm32wlxx_hal_flash.su ./Src/stm32wlxx_hal_flash_ex.cyclo ./Src/stm32wlxx_hal_flash_ex.d ./Src/stm32wlxx_hal_flash_ex.o ./Src/stm32wlxx_hal_flash_ex.su ./Src/stm32wlxx_hal_gpio.cyclo ./Src/stm32wlxx_hal_gpio.d ./Src/stm32wlxx_hal_gpio.o ./Src/stm32wlxx_hal_gpio.su ./Src/stm32wlxx_hal_gtzc.cyclo ./Src/stm32wlxx_hal_gtzc.d ./Src/stm32wlxx_hal_gtzc.o ./Src/stm32wlxx_hal_gtzc.su ./Src/stm32wlxx_hal_hsem.cyclo ./Src/stm32wlxx_hal_hsem.d ./Src/stm32wlxx_hal_hsem.o ./Src/stm32wlxx_hal_hsem.su ./Src/stm32wlxx_hal_i2c.cyclo ./Src/stm32wlxx_hal_i2c.d ./Src/stm32wlxx_hal_i2c.o ./Src/stm32wlxx_hal_i2c.su ./Src/stm32wlxx_hal_i2c_ex.cyclo ./Src/stm32wlxx_hal_i2c_ex.d ./Src/stm32wlxx_hal_i2c_ex.o ./Src/stm32wlxx_hal_i2c_ex.su ./Src/stm32wlxx_hal_i2s.cyclo ./Src/stm32wlxx_hal_i2s.d ./Src/stm32wlxx_hal_i2s.o ./Src/stm32wlxx_hal_i2s.su ./Src/stm32wlxx_hal_ipcc.cyclo ./Src/stm32wlxx_hal_ipcc.d ./Src/stm32wlxx_hal_ipcc.o ./Src/stm32wlxx_hal_ipcc.su ./Src/stm32wlxx_hal_irda.cyclo ./Src/stm32wlxx_hal_irda.d ./Src/stm32wlxx_hal_irda.o ./Src/stm32wlxx_hal_irda.su ./Src/stm32wlxx_hal_iwdg.cyclo ./Src/stm32wlxx_hal_iwdg.d ./Src/stm32wlxx_hal_iwdg.o ./Src/stm32wlxx_hal_iwdg.su ./Src/stm32wlxx_hal_lptim.cyclo ./Src/stm32wlxx_hal_lptim.d ./Src/stm32wlxx_hal_lptim.o ./Src/stm32wlxx_hal_lptim.su ./Src/stm32wlxx_hal_msp.cyclo ./Src/stm32wlxx_hal_msp.d ./Src/stm32wlxx_hal_msp.o ./Src/stm32wlxx_hal_msp.su ./Src/stm32wlxx_hal_pka.cyclo ./Src/stm32wlxx_hal_pka.d ./Src/stm32wlxx_hal_pka.o ./Src/stm32wlxx_hal_pka.su ./Src/stm32wlxx_hal_pwr.cyclo ./Src/stm32wlxx_hal_pwr.d ./Src/stm32wlxx_hal_pwr.o ./Src/stm32wlxx_hal_pwr.su ./Src/stm32wlxx_hal_pwr_ex.cyclo ./Src/stm32wlxx_hal_pwr_ex.d ./Src/stm32wlxx_hal_pwr_ex.o ./Src/stm32wlxx_hal_pwr_ex.su ./Src/stm32wlxx_hal_rcc.cyclo ./Src/stm32wlxx_hal_rcc.d ./Src/stm32wlxx_hal_rcc.o ./Src/stm32wlxx_hal_rcc.su ./Src/stm32wlxx_hal_rcc_ex.cyclo ./Src/stm32wlxx_hal_rcc_ex.d ./Src/stm32wlxx_hal_rcc_ex.o ./Src/stm32wlxx_hal_rcc_ex.su ./Src/stm32wlxx_hal_rng.cyclo ./Src/stm32wlxx_hal_rng.d ./Src/stm32wlxx_hal_rng.o ./Src/stm32wlxx_hal_rng.su ./Src/stm32wlxx_hal_rng_ex.cyclo ./Src/stm32wlxx_hal_rng_ex.d ./Src/stm32wlxx_hal_rng_ex.o ./Src/stm32wlxx_hal_rng_ex.su ./Src/stm32wlxx_hal_rtc.cyclo ./Src/stm32wlxx_hal_rtc.d ./Src/stm32wlxx_hal_rtc.o ./Src/stm32wlxx_hal_rtc.su ./Src/stm32wlxx_hal_rtc_ex.cyclo ./Src/stm32wlxx_hal_rtc_ex.d ./Src/stm32wlxx_hal_rtc_ex.o ./Src/stm32wlxx_hal_rtc_ex.su ./Src/stm32wlxx_hal_smartcard.cyclo ./Src/stm32wlxx_hal_smartcard.d ./Src/stm32wlxx_hal_smartcard.o ./Src/stm32wlxx_hal_smartcard.su ./Src/stm32wlxx_hal_smartcard_ex.cyclo ./Src/stm32wlxx_hal_smartcard_ex.d ./Src/stm32wlxx_hal_smartcard_ex.o ./Src/stm32wlxx_hal_smartcard_ex.su ./Src/stm32wlxx_hal_smbus.cyclo ./Src/stm32wlxx_hal_smbus.d ./Src/stm32wlxx_hal_smbus.o ./Src/stm32wlxx_hal_smbus.su ./Src/stm32wlxx_hal_smbus_ex.cyclo ./Src/stm32wlxx_hal_smbus_ex.d ./Src/stm32wlxx_hal_smbus_ex.o ./Src/stm32wlxx_hal_smbus_ex.su ./Src/stm32wlxx_hal_spi.cyclo ./Src/stm32wlxx_hal_spi.d ./Src/stm32wlxx_hal_spi.o ./Src/stm32wlxx_hal_spi.su ./Src/stm32wlxx_hal_spi_ex.cyclo ./Src/stm32wlxx_hal_spi_ex.d ./Src/stm32wlxx_hal_spi_ex.o ./Src/stm32wlxx_hal_spi_ex.su ./Src/stm32wlxx_hal_subghz.cyclo ./Src/stm32wlxx_hal_subghz.d ./Src/stm32wlxx_hal_subghz.o ./Src/stm32wlxx_hal_subghz.su ./Src/stm32wlxx_hal_tim.cyclo ./Src/stm32wlxx_hal_tim.d ./Src/stm32wlxx_hal_tim.o ./Src/stm32wlxx_hal_tim.su ./Src/stm32wlxx_hal_tim_ex.cyclo ./Src/stm32wlxx_hal_tim_ex.d ./Src/stm32wlxx_hal_tim_ex.o ./Src/stm32wlxx_hal_tim_ex.su ./Src/stm32wlxx_hal_uart.cyclo ./Src/stm32wlxx_hal_uart.d ./Src/stm32wlxx_hal_uart.o ./Src/stm32wlxx_hal_uart.su ./Src/stm32wlxx_hal_uart_ex.cyclo ./Src/stm32wlxx_hal_uart_ex.d ./Src/stm32wlxx_hal_uart_ex.o ./Src/stm32wlxx_hal_uart_ex.su ./Src/stm32wlxx_hal_usart.cyclo ./Src/stm32wlxx_hal_usart.d ./Src/stm32wlxx_hal_usart.o ./Src/stm32wlxx_hal_usart.su ./Src/stm32wlxx_hal_usart_ex.cyclo ./Src/stm32wlxx_hal_usart_ex.d ./Src/stm32wlxx_hal_usart_ex.o ./Src/stm32wlxx_hal_usart_ex.su
	-$(RM) ./Src/stm32wlxx_hal_wwdg.cyclo ./Src/stm32wlxx_hal_wwdg.d ./Src/stm32wlxx_hal_wwdg.o ./Src/stm32wlxx_hal_wwdg.su ./Src/stm32wlxx_ll_adc.cyclo ./Src/stm32wlxx_ll_adc.d ./Src/stm32wlxx_ll_adc.o ./Src/stm32wlxx_ll_adc.su ./Src/stm32wlxx_ll_comp.cyclo ./Src/stm32wlxx_ll_comp.d ./Src/stm32wlxx_ll_comp.o ./Src/stm32wlxx_ll_comp.su ./Src/stm32wlxx_ll_crc.cyclo ./Src/stm32wlxx_ll_crc.d ./Src/stm32wlxx_ll_crc.o ./Src/stm32wlxx_ll_crc.su ./Src/stm32wlxx_ll_dac.cyclo ./Src/stm32wlxx_ll_dac.d ./Src/stm32wlxx_ll_dac.o ./Src/stm32wlxx_ll_dac.su ./Src/stm32wlxx_ll_dma.cyclo ./Src/stm32wlxx_ll_dma.d ./Src/stm32wlxx_ll_dma.o ./Src/stm32wlxx_ll_dma.su ./Src/stm32wlxx_ll_exti.cyclo ./Src/stm32wlxx_ll_exti.d ./Src/stm32wlxx_ll_exti.o ./Src/stm32wlxx_ll_exti.su ./Src/stm32wlxx_ll_gpio.cyclo ./Src/stm32wlxx_ll_gpio.d ./Src/stm32wlxx_ll_gpio.o ./Src/stm32wlxx_ll_gpio.su ./Src/stm32wlxx_ll_i2c.cyclo ./Src/stm32wlxx_ll_i2c.d ./Src/stm32wlxx_ll_i2c.o ./Src/stm32wlxx_ll_i2c.su ./Src/stm32wlxx_ll_lptim.cyclo ./Src/stm32wlxx_ll_lptim.d ./Src/stm32wlxx_ll_lptim.o ./Src/stm32wlxx_ll_lptim.su ./Src/stm32wlxx_ll_lpuart.cyclo ./Src/stm32wlxx_ll_lpuart.d ./Src/stm32wlxx_ll_lpuart.o ./Src/stm32wlxx_ll_lpuart.su ./Src/stm32wlxx_ll_pka.cyclo ./Src/stm32wlxx_ll_pka.d ./Src/stm32wlxx_ll_pka.o ./Src/stm32wlxx_ll_pka.su ./Src/stm32wlxx_ll_pwr.cyclo ./Src/stm32wlxx_ll_pwr.d ./Src/stm32wlxx_ll_pwr.o ./Src/stm32wlxx_ll_pwr.su ./Src/stm32wlxx_ll_rcc.cyclo ./Src/stm32wlxx_ll_rcc.d ./Src/stm32wlxx_ll_rcc.o ./Src/stm32wlxx_ll_rcc.su ./Src/stm32wlxx_ll_rng.cyclo ./Src/stm32wlxx_ll_rng.d ./Src/stm32wlxx_ll_rng.o ./Src/stm32wlxx_ll_rng.su ./Src/stm32wlxx_ll_rtc.cyclo ./Src/stm32wlxx_ll_rtc.d ./Src/stm32wlxx_ll_rtc.o ./Src/stm32wlxx_ll_rtc.su ./Src/stm32wlxx_ll_spi.cyclo ./Src/stm32wlxx_ll_spi.d ./Src/stm32wlxx_ll_spi.o ./Src/stm32wlxx_ll_spi.su ./Src/stm32wlxx_ll_tim.cyclo ./Src/stm32wlxx_ll_tim.d ./Src/stm32wlxx_ll_tim.o ./Src/stm32wlxx_ll_tim.su ./Src/stm32wlxx_ll_usart.cyclo ./Src/stm32wlxx_ll_usart.d ./Src/stm32wlxx_ll_usart.o ./Src/stm32wlxx_ll_usart.su ./Src/stm32wlxx_ll_utils.cyclo ./Src/stm32wlxx_ll_utils.d ./Src/stm32wlxx_ll_utils.o ./Src/stm32wlxx_ll_utils.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32wlxx.cyclo ./Src/system_stm32wlxx.d ./Src/system_stm32wlxx.o ./Src/system_stm32wlxx.su

.PHONY: clean-Src

