################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Drivers/ECC/ECCROMCC26XX.obj: C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx/ecc/ECCROMCC26XX.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" --cmd_file="C:/ti/simplelink/ble_sdk_2_02_00_31/examples/cc2650lp/simple_broadcaster/ccs/app/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="C:/ti/simplelink/ble_sdk_2_02_00_31/examples/cc2650lp/simple_broadcaster/ccs/app/../../iar/stack/build_config.opt" --cmd_file="C:/ti/simplelink/ble_sdk_2_02_00_31/examples/cc2650lp/simple_broadcaster/ccs/app/../../iar/stack/../../ccs/config/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 --abi=eabi -me -O1 --opt_for_speed=0 --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Application" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/BSP_Drivers" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Startup" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/ICall" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/ICallBLE" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Include" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/PROFILES" --include_path="C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/TOOLS" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/icall/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/profiles/roles/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/profiles/roles" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/heapmgr" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/controller/cc26xx/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/target" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common/cc26xx" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/osal/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/sdata" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/saddr" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/icall/src/inc" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc" --include_path="C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/cc26xxware_2_23_03_17162" --include_path="C:/ti/simplelink/ble_sdk_2_02_00_31/src/examples/simple_broadcaster/cc26xx/app" --c99 --define=USE_ICALL --define=SBB_TASK_STACK_SIZE=700 --define=GAPROLE_TASK_STACK_SIZE=520 --define=POWER_SAVING --define=Display_DISABLE_ALL --define=BOARD_DISPLAY_EXCLUDE_UART --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_MAX_NUM_ENTITIES=6 --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL --define=CC2650_LAUNCHXL --define=CC26XX --diag_warning=225 --diag_suppress=48 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Drivers/ECC/ECCROMCC26XX.d_raw" --obj_directory="Drivers/ECC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


