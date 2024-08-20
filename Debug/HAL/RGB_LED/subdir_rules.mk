################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
HAL/RGB_LED/%.obj: ../HAL/RGB_LED/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/IDEs/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL/ADC" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL/SYSCTL" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/HAL" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/HAL/RGB_LED" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/Common" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/FreeRTOS/Source/include" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL/GPIO" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL/GPTM" --include_path="D:/Career/Embedded Systems/RTOS Course/Final Project/SeatHeater/MCAL/UART" --include_path="D:/IDEs/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="HAL/RGB_LED/$(basename $(<F)).d_raw" --obj_directory="HAL/RGB_LED" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


