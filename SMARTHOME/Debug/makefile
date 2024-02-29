################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/USART/subdir.mk
-include MCAL/TIMERS/TIMER1/subdir.mk
-include MCAL/TIMERS/TIMER0/subdir.mk
-include MCAL/SPI/subdir.mk
-include MCAL/I2C/subdir.mk
-include MCAL/GIE/subdir.mk
-include MCAL/EXTI/subdir.mk
-include MCAL/DIO/subdir.mk
-include MCAL/ADC/subdir.mk
-include HAL/Stepper_Motor/subdir.mk
-include HAL/SERVO_MOTOR/subdir.mk
-include HAL/LED/subdir.mk
-include HAL/LCD/subdir.mk
-include HAL/KPAD/subdir.mk
-include HAL/EEPROM/subdir.mk
-include HAL/Button/subdir.mk
-include HAL/7-Segment/subdir.mk
-include APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
SMARTHOME.lss \

FLASH_IMAGE += \
SMARTHOME.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: SMARTHOME.elf secondary-outputs

# Tool invocations
SMARTHOME.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,SMARTHOME.map -mmcu=atmega32 -o "SMARTHOME.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

SMARTHOME.lss: SMARTHOME.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S SMARTHOME.elf  >"SMARTHOME.lss"
	@echo 'Finished building: $@'
	@echo ' '

SMARTHOME.hex: SMARTHOME.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex SMARTHOME.elf  "SMARTHOME.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: SMARTHOME.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 SMARTHOME.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) SMARTHOME.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
