################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Libraries/%.obj: ../Libraries/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccs1100/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/ti/ccs1100/ccs/ccs_base/msp430/include" --include_path="E:/Current Works/ToF10120_LIDAR_Demo/LIDAR_demo/Libraries" --include_path="E:/Current Works/ToF10120_LIDAR_Demo/LIDAR_demo" --include_path="D:/ti/ccs1100/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power=all --define=__MSP430G2452__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Libraries/$(basename $(<F)).d_raw" --obj_directory="Libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


