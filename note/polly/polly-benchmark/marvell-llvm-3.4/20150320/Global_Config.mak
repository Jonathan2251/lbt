## =============================================================================
##
# Build Setting
##
## =============================================================================
## Select benchmark you want to build. {
# bms/clinpack
# bms/coremark
# bms/dhrystone
# bms/eembc_1.1
# bms/eembc_networking_2.0
# bms/eembc_oa_2.0
# bms/llcbench
# bms/lmbench3
# bms/multibench-mini1e
# bms/STREAM
# bms/whetstone
#}.
TEST_BENCHMARKS := bms/clinpack

## Build Rules. Permissible values are: { opt | otb | o3 | none }
BUILD_RULE := o3

## OS platform selection. Permissible values are: { eabi | linux }
PLATFORM := linux

## Initial-file included or not for bare-metal environment. Permissible values are: { 0 | 1 }
## This variable can be ignored while the PLATFORM is set to linux.
INCLUDE_INIT := 1

## retarget functions for eabi Permissible values are: {none | dove | rtlsim | tahoe}
RETARGET := none

## For linux build, but use eabi option file: { 0 | 1 }
USE_EABI_OPT := 0

## Time measurement method. Permissible values are: { pmu | timer }
TIME := timer

## Instruction set selection for arm. Permissible values are: { 0 | 1 }
ENABLE_THUMB := 0

## Using profile-guided optimization (PGO). Permissible values are: { 0 | 1 }
USE_PGO := 1

## Using link time optimization (LTO), only for gcc 4.4+. Permissible values are: { 0 | 1 }"
USE_LTO := 0

## Toolchain config. Select one config file in build/config/toolchain {
# hostgcc.mak
# mrvlgcc4.4_2010q4.mak
# mrvlgcc4.4_201301.mak
# mrvlgcc4.6_201301.mak
# mrvlgcc4.6_201307.mak
# mrvlgcc4.6_201404.mak
# mrvlgcc4.8_201307.mak
# mrvlgcc4.8_201407.mak
# mrvlgcc4.8_dev.mak
# mrvlgcc4.9_aarch64_dev.mak
# mrvlllvm3.4.mak
# rvct.mak
#}.
TOOLCHAIN_MAK := mrvlllvm3.4.mak

## Board directory. Select one target board folder in build/config/board {
# aarch64_foundation_model
# armadaxp
# baremetal_example
# doveA0
# host
# rtlsim
# versatileA15
# whitney-FPGA
#}.
BOARD_DIR := armadaxp

## You CPU frequency (MHz). Using pmu to caculate performance will divide by frequency.
CORE_FREQ := 1000

## Toolchain path setting.
## Example: /home/neilh/work/gcc/mrvl_gcc4.6/Release/install/armv7-marvell-linux-gnueabi-hard_i686/bin/arm-marvell-linux-gnueabi-
CROSS_COMPILE := /home/cschen/test/2/llvm-3.4-arm-hard-x86_64-201410/bin/arm-linux-gnueabihf-
#CROSS_COMPILE := /home/cschen/test/polly/cmake_arm_hard1_debug_build/bin/
#CROSS_COMPILE := /home/cschen/test/polly/cmake_arm_debug_build/bin/

##########################  ADVANCED OPTION  ##########################

## If you define CROSS_COMPILE, you should have not to set following variable
## For example:
##   CC: default is ${CROSS_COMPILE}gcc for gcc
##   CC: default is ${CROSS_COMPILE}clang for llvm
##   CC: default is ${CROSS_COMPILE}armcc for rvct
##   CXX: default is ${CROSS_COMPILE}g++ for gcc
##   CXX: default is ${CROSS_COMPILE}clang++ for llvm
##   CXX: default is ${CROSS_COMPILE}armcc --cpp for rvct
## If you set follow variable, that will override default setting.
## If you don't want to change default value, keep them empty
CC :=
CXX :=
OBJDUMP :=
OBJCOPY :=
AR :=
AS :=
LD :=
SIZE :=
RANLIB :=

# set CPU option, default measns using option file
# Example: CPU="-mcpu=marvell-pj4b -mfpu=neon"
CPU := default

# set MRVL_HW option, default means using option file
# on means turn on , off means turn off, or you can set by your self
# Example: MRVL_HW=[ on | off | default | <others>]
MRVL_HW := default

# Extra option append to compile flag"
# example:  XCFLAGS="-g -save-temps"
#XCFLAGS := -Xclang -load -Xclang ~/test/polly/cmake_arm_hard1_debug_build/lib/LLVMPolly.so -mllvm -polly -mllvm -polly-ignore-aliasing
#XCFLAGS := -Xclang -load -Xclang -mllvm -polly -mllvm -polly-ignore-aliasing
XCFLAGS :=

# Extra option append to link flag
# Example:  XLDFLAGS="-Wl,--verbose"
XLDFLAGS :=

