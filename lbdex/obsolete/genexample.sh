#!/usr/bin/env bash
GEN_DIR=lbdex
rm -rf Chapter3_1 Chapter3_2 Chapter3_2 Chapter3_4 Chapter3_5
rm -rf Chapter4_1 Chapter4_2 
rm -rf Chapter5_1 Chapter6_1 Chapter7_1 Chapter8_1 Chapter8_2
rm -rf Chapter9_1 Chapter9_2 Chapter9_3 Chapter9_4
rm -rf Chapter10_1 Chapter11_1 Chapter11_2 Chapter14_1
rm -rf ${GEN_DIR}
mkdir ${GEN_DIR}
cp -rf Chapter2 ${GEN_DIR}/.
patch -fp0 < 3-1.patch
mv Chapter2 Chapter3_1
cp -rf Chapter3_1 ${GEN_DIR}/.
patch -fp0 < 3-2.patch
mv Chapter3_1 Chapter3_2
cp -rf Chapter3_2 ${GEN_DIR}/.
patch -fp0 < 3-3.patch
mv Chapter3_2 Chapter3_3
cp -rf Chapter3_3 ${GEN_DIR}/.
patch -fp0 < 3-4.patch
mv Chapter3_3 Chapter3_4
cp -rf Chapter3_4 ${GEN_DIR}/.
patch -fp0 < 3-5.patch
mv Chapter3_4 Chapter3_5
cp -rf Chapter3_5 ${GEN_DIR}/.
patch -fp0 < 3-6.patch
mv Chapter3_5 Chapter3_6
cp -rf Chapter3_6 ${GEN_DIR}/.
patch -fp0 < 4-1.patch
mv Chapter3_6 Chapter4_1
cp -rf Chapter4_1 ${GEN_DIR}/.
patch -fp0 < 4-2.patch
mv Chapter4_1 Chapter4_2
cp -rf Chapter4_2 ${GEN_DIR}/.
patch -fp0 < 5-1.patch
mv Chapter4_2 Chapter5_1
cp -rf Chapter5_1 ${GEN_DIR}/.
patch -fp0 < 6-1.patch
mv Chapter5_1 Chapter6_1
cp -rf Chapter6_1 ${GEN_DIR}/.
patch -fp0 < 7-1.patch
mv Chapter6_1 Chapter7_1
cp -rf Chapter7_1 ${GEN_DIR}/.
patch -fp0 < 8-1.patch
mv Chapter7_1 Chapter8_1
cp -rf Chapter8_1 ${GEN_DIR}/.
patch -fp0 < 8-2.patch
mv Chapter8_1 Chapter8_2
cp -rf Chapter8_2 ${GEN_DIR}/.
patch -fp0 < 9-1.patch
mv Chapter8_2 Chapter9_1
cp -rf Chapter9_1 ${GEN_DIR}/.
patch -fp0 < 9-2.patch
mv Chapter9_1 Chapter9_2
cp -rf Chapter9_2 ${GEN_DIR}/.
patch -fp0 < 9-3.patch
mv Chapter9_2 Chapter9_3
cp -rf Chapter9_3 ${GEN_DIR}/.
patch -fp0 < 9-4.patch
mv Chapter9_3 Chapter9_4
cp -rf Chapter9_4 ${GEN_DIR}/.
patch -fp0 < 10-1.patch
mv Chapter9_4 Chapter10_1
cp -rf Chapter10_1 ${GEN_DIR}/.
patch -fp0 < 11-1.patch
mv Chapter10_1 Chapter11_1
cp -rf Chapter11_1 ${GEN_DIR}/.
patch -fp0 < 11-2.patch
mv Chapter11_1 Chapter11_2
cp -rf Chapter11_2 ${GEN_DIR}/.
patch -fp0 < 14-1.patch
mv Chapter11_2 Chapter14_1
cp -rf Chapter14_1 ${GEN_DIR}/.
rm -rf Chapter14_1
cp -rf ${GEN_DIR}/Chapter2 .
cp -rf InputFiles src_files_modify ${GEN_DIR}/.
cp -rf src_files_modify Cpu0_lld llvm-objdump ${GEN_DIR}/.
cp -rf cpu0_verilog ${GEN_DIR}/.
cp -rf removecpu0.sh readme ${GEN_DIR}/.
rm -f `find . -name .DS_Store`
