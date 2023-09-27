APP_NAME := hello_world

# paths
# DE2-35 sources
CORE_FILE := ../../hardware/DE2-pre-built/DE2_Nios2System.sopcinfo
SOF_FILE  := ../../hardware/DE2-pre-built/IL2206_DE2_Nios2.sof
JDI_FILE  := ../../hardware/DE2-pre-built/IL2206_DE2_Nios2.jdi
# DE2-115 sources
# CORE_FILE := ../../hardware/DE2-115-pre-built/DE2_115_Nios2System.sopcinfo
# SOF_FILE  := ../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.sof
# JDI_FILE  := ../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.jdi

BSP_PATH := ../../bsp/il2206-pre-built
SRC_PATH := src
GEN_PATH := gen
BIN_PATH := bin

# component names
CPU_NAME := nios2
BSP_TYPE := hal

# commands
NIOS2_BSP_COMMANDS := --set hal.make.bsp_cflags_debug -g \
	  --set hal.make.bsp_cflags_optimization '-Os' \
	  --set hal.enable_sopc_sysid_check true \
	  --set hal.enable_reduced_device_drivers true \
	  --default_sections_mapping sram \
	  --set hal.enable_small_c_library true \
	  --set hal.sys_clk_timer none \
	  --set hal.timestamp_timer none \
	  --set hal.enable_exit false \
	  --set hal.enable_c_plus_plus false \
	  --set hal.enable_lightweight_device_driver_api true \
	  --set hal.enable_clean_exit false \
	  --set hal.max_file_descriptors 4 \
	  --set hal.enable_sim_optimize false

MAKEFILE_COMMANDS := --set APP_CFLAGS_OPTIMIZATION -Os

include ../Makefile.in
