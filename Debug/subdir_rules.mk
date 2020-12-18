################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=1 --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include" --include_path="C:/ti/msp432_driverlib_3_21_00_05/driverlib/MSP432P4xx" --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Juan Francisco Valle/OneDrive - Georgia Institute of Technology/Senior/Mechatronics/CCS_workspace/Mini_Segway" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --rtti --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --gen_preprocessor_listing --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=1 --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include" --include_path="C:/ti/msp432_driverlib_3_21_00_05/driverlib/MSP432P4xx" --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Juan Francisco Valle/OneDrive - Georgia Institute of Technology/Senior/Mechatronics/CCS_workspace/Mini_Segway" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.6.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --rtti --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --gen_preprocessor_listing --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


