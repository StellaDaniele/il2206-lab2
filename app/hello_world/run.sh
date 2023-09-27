#!/bin/bash
# @file: run.sh
# @author: George Ungureanu, KTH/EECS/ELE
# @date: 21.08.2018
# @version: 0.1
#
# This is a bash script for automating the compilation and deployment
# of the Nios II project in the current folder. It is a more readable
# (albeit less powerful) version of the 'Makefile' one folder
# above. It is recommended for beginner students to understand what is
# happening during the compilation process.

# Paths for DE2-115 sources
# CORE_FILE=../../hardware/DE2-115-pre-built/DE2_115_Nios2System.sopcinfo
# SOF_FILE=../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.sof
# JDI_FILE=../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.jdi

# Paths for DE2-35 sources
CORE_FILE=../../hardware/DE2-pre-built/DE2_Nios2System.sopcinfo
SOF_FILE=../../hardware/DE2-pre-built/IL2206_DE2_Nios2.sof
JDI_FILE=../../hardware/DE2-pre-built/IL2206_DE2_Nios2.jdi

APP_NAME=hello_world
CPU_NAME=nios2
BSP_PATH=../../bsp/il2206-pre-built
SRC_PATH=./src

# Project internal folders
GEN=gen
BIN=bin
mkdir -p $GEN
mkdir -p $BIN

echo -e "\n******************************************"
echo -e   "Building the BSP and compiling the program"
echo -e   "******************************************\n"

nios2-bsp hal $BSP_PATH $CORE_FILE \
	  --cpu-name $CPU_NAME \
	  --set hal.make.bsp_cflags_debug -g \
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

cd $GEN
nios2-app-generate-makefile \
    --bsp-dir ../$BSP_PATH \
    --elf-name ../$BIN/$APP_NAME.elf \
    --src-dir ../$SRC_PATH \
    --set APP_CFLAGS_OPTIMIZATION -O0

make 3>&1 1>>log.txt 2>&1
cd ..

echo -e "\n**************************"
echo -e  "Download hardware to board"
echo -e  "**************************\n"

nios2-configure-sof $SOF_FILE

echo -e "\n**************************"
echo -e   "Download software to board"
echo -e   "**************************\n"

xterm -e "nios2-terminal -i 0" &
nios2-download -g $BIN/$APP_NAME.elf --cpu_name $CPU_NAME --jdi $JDI_FILE


echo ""
echo "Code compilation errors are logged in 'gen/log.txt'"
