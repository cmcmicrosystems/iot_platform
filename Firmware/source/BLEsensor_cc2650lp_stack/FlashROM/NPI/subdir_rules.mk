################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
NPI/npi.obj: C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx/npi/stack/npi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" --cmd_file="C:/ti/simplelink/ble_sdk_2_02_00_31/examples/cc2650lp/simple_broadcaster/ccs/stack/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="C:/ti/simplelink/ble_sdk_2_02_00_31/examples/cc2650lp/simple_broadcaster/ccs/stack/../../iar/stack/build_config.opt"  -mv7M3 --code_state=16 --abi=eabi -me -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/examples/simple_broadcaster/cc26xx/stack" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/osal/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/saddr" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/nv/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/nv" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/icall/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/controller/cc26xx/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/aes/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/npi/src" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx/npi/stack" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/icall/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/profiles/roles" --include_path="C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/cc26xxware_2_23_03_17162" --c99 --define=USE_ICALL --define=FLASH_ROM_BUILD --define=POWER_SAVING --define=OSAL_SNV=1 --define=INCLUDE_AES_DECRYPT --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --define=OSAL_CBTIMER_NUM_TASKS=1 --define=EXT_HAL_ASSERT --define=xDEBUG_GPIO --define=xDEBUG_ENC --define=xDEBUG_SW_TRACE --define=NEAR_FUNC= --define=DATA= --define=CC26XXWARE --define=CC26XX --diag_warning=225 --diag_suppress=48 --diag_suppress=16004 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="NPI/npi.d_raw" --obj_directory="NPI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


