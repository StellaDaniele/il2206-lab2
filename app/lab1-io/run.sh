#!/bin/bash
# @file: run.sh
# @authors: Rodolfo Jordao, KTH/EECS/ELE
#           George Ungureanu, KTH/EECS/ELE
# @date: 2019-08-20
# @version: 0.2
#
# This is a bash script for automating the compilation and deployment
# of the Nios II project in the current folder. It is a more readable
# (albeit less powerful) version of the 'Makefile' one folder
# above. It is recommended for beginner students to understand what is
# happening during the compilation process.

# Paths for DE2-35 sources
CORE_FILE=../../hardware/DE2-pre-built/DE2_Nios2System.sopcinfo
SOF_FILE=../../hardware/DE2-pre-built/IL2206_DE2_Nios2.sof
JDI_FILE=../../hardware/DE2-pre-built/IL2206_DE2_Nios2.jdi

# Paths for DE2-115 sources
# CORE_FILE=../../hardware/DE2-115-pre-built/DE2_115_Nios2System.sopcinfo
# SOF_FILE=../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.sof
# JDI_FILE=../../hardware/DE2-115-pre-built/IL2206_DE2_115_Nios2.jdi

APP_NAME=lab1-io
CPU_NAME=nios2
BSP_PATH=../../bsp/il2206-pre-built
SRC_PATH=./src


echo -e "\n******************************************"
echo -e   "Building the BSP and compiling the program"
echo -e   "******************************************\n"

cp -r $BSP_PATH bsp

nios2-bsp hal bsp $CORE_FILE \
	  --cpu-name $CPU_NAME \
	  --set hal.make.bsp_cflags_debug -g \
	  --set hal.make.bsp_cflags_optimization -O0 \
	  --set hal.enable_sopc_sysid_check 1 \
	  --set hal.max_file_descriptors 4

nios2-app-generate-makefile \
    --bsp-dir bsp \
    --elf-name $APP_NAME.elf \
    --src-dir $SRC_PATH \
    --set APP_CFLAGS_OPTIMIZATION -O0

make | tee -a log.txt

echo -e "\n**************************"
echo -e  "Download hardware to board"
echo -e  "**************************\n"

nios2-configure-sof $SOF_FILE


echo -e "\n**************************"
echo -e   "Download software to board"
echo -e   "**************************\n"

xterm -e "nios2-terminal -i 0" &
nios2-download -g $APP_NAME.elf --cpu_name $CPU_NAME --jdi $JDI_FILE


echo ""
echo "Code compilation errors are logged in 'log.txt'"
