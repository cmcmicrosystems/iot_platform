################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-287986657:
	@$(MAKE) --no-print-directory -Onone -f TOOLS/subdir_rules.mk build-287986657-inproc

build-287986657-inproc: ../TOOLS/app_ble.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_32_00_06_core/xs" --xdcpath="C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/packages;C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/tidrivers_cc13xx_cc26xx_2_16_01_13/packages;C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/bios_6_45_02_31/packages;C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/uia_2_00_05_50/packages;C:/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640F128 -r release -c "C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6" --compileOptions "-mv7M3 --code_state=16 --abi=eabi -me -O1 --opt_for_speed=0 --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Application\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/BSP_Drivers\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Startup\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/ICall\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/ICallBLE\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/Include\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/PROFILES\" --include_path=\"C:/Users/Olive/workspace_v7/BLEsensor_cc2650lp_app/TOOLS\" --include_path=\"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_5.2.6/include\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/icall/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/profiles/roles/cc26xx\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/profiles/roles\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/common/cc26xx\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/heapmgr\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/controller/cc26xx/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/target\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/target/_common/cc26xx\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/hal/src/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/osal/src/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/sdata\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/services/src/saddr\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/components/icall/src/inc\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/inc\" --include_path=\"C:/ti/tirtos_cc13xx_cc26xx_2_18_00_03/products/cc26xxware_2_23_03_17162\" --include_path=\"C:/ti/simplelink/ble_sdk_2_02_00_31/src/examples/simple_broadcaster/cc26xx/app\" --c99 --define=USE_ICALL --define=SBB_TASK_STACK_SIZE=700 --define=GAPROLE_TASK_STACK_SIZE=520 --define=POWER_SAVING --define=Display_DISABLE_ALL --define=BOARD_DISPLAY_EXCLUDE_UART --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_MAX_NUM_ENTITIES=6 --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL --define=CC2650_LAUNCHXL --define=CC26XX --diag_warning=225 --diag_suppress=48 --diag_wrap=off --display_error_number --gen_func_subsections=on " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-287986657 ../TOOLS/app_ble.cfg
configPkg/compiler.opt: build-287986657
configPkg/: build-287986657


