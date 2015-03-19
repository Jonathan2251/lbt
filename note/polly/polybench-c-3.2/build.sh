#!/bin/bash -a

rm -rf polybench
mkdir polybench

BUILD_ARM=0
#BUILD_ARM=1

#DUMP="-DPOLYBENCH_DUMP_ARRAYS"
DUMP=""

#DATASET=SMALL_DATASET
#DATASET=STANDARD_DATASET
DATASET=LARGE_DATASET

if [ ${BUILD_ARM} == 1 ]; then
CPU="-mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon"
LLVM_INSTALL=~/test/0618-polly/llvm_arm_build
#LLVM_INSTALL=~/test/polly/llvm_arm_build
else
CPU=""
LLVM_INSTALL=~/test/0618-polly/llvm_build
#LLVM_INSTALL=~/test/0417-polly/llvm_build
#LLVM_INSTALL=~/test/polly/llvm_build
fi

CFLAG="-O3 -I utilities -I datamining/covariance utilities/polybench.c -DPOLYBENCH_TIME -DPOLYBENCH_USE_SCALAR_LB -D${DATASET} -lm ${DUMP} ${CPU}"
PCFLAG="-O3 -mllvm -polly -mllvm -polly-ignore-aliasing -I utilities -I datamining/covariance utilities/polybench.c -DPOLYBENCH_TIME -DPOLYBENCH_USE_SCALAR_LB -D${DATASET} -lm ${DUMP} ${CPU}"
PCFLAGOMP="-O3 -mllvm -polly -mllvm -enable-polly-openmp -lgomp -mllvm -polly-ignore-aliasing -I utilities -I datamining/covariance utilities/polybench.c -DPOLYBENCH_TIME -DPOLYBENCH_USE_SCALAR_LB -D${DATASET} -lm ${DUMP} ${CPU}"

pollycc="${LLVM_INSTALL}/bin/clang -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"

${LLVM_INSTALL}/bin/clang ${CFLAG} datamining/correlation/correlation.c -o polybench/correlation
${LLVM_INSTALL}/bin/clang ${CFLAG} datamining/covariance/covariance.c -o polybench/covariance

${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/2mm/2mm.c -o polybench/2mm
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/2mm/ex11_41.c -o polybench/ex11_41
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/3mm/3mm.c -o polybench/3mm
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/atax/atax.c -o polybench/atax
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/bicg/bicg.c -o polybench/bicg
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/cholesky/cholesky.c -o polybench/cholesky
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/doitgen/doitgen.c -o polybench/doitgen
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/gemm/gemm.c -o polybench/gemm
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/gemver/gemver.c -o polybench/gemver
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/gesummv/gesummv.c -o polybench/gesummv
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/mvt/mvt.c -o polybench/mvt
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/symm/symm.c -o polybench/symm
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/syr2k/syr2k.c -o polybench/syr2k
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/syrk/syrk.c -o polybench/syrk
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/trisolv/trisolv.c -o polybench/trisolv
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/kernels/trmm/trmm.c -o polybench/trmm

${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/solvers/durbin/durbin.c -o polybench/durbin
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/solvers/dynprog/dynprog.c -o polybench/dynprog
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/solvers/gramschmidt/gramschmidt.c -o polybench/gramschmidt
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/solvers/lu/lu.c -o polybench/lu
${LLVM_INSTALL}/bin/clang ${CFLAG} linear-algebra/solvers/ludcmp/ludcmp.c -o polybench/ludcmp

${LLVM_INSTALL}/bin/clang ${CFLAG} medley/floyd-warshall/floyd-warshall.c -o polybench/floyd-warshall
${LLVM_INSTALL}/bin/clang ${CFLAG} medley/reg_detect/reg_detect.c -o polybench/reg_detect

${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/adi/adi.c -o polybench/adi
${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/fdtd-2d/fdtd-2d.c -o polybench/fdtd-2d
${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/fdtd-apml/fdtd-apml.c -o polybench/fdtd-apml
${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/jacobi-1d-imper/jacobi-1d-imper.c -o polybench/jacobi-1d-imper
${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/jacobi-2d-imper/jacobi-2d-imper.c -o polybench/jacobi-2d-imper
${LLVM_INSTALL}/bin/clang ${CFLAG} stencils/seidel-2d/seidel-2d.c -o polybench/seidel-2d



${pollycc} ${PCFLAG} datamining/correlation/correlation.c -o polybench/correlation.tile
${pollycc} ${PCFLAG} datamining/covariance/covariance.c -o polybench/covariance.tile

${pollycc} ${PCFLAG} linear-algebra/kernels/2mm/2mm.c -o polybench/2mm.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/2mm/ex11_41.c -o polybench/ex11_41.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/3mm/3mm.c -o polybench/3mm.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/atax/atax.c -o polybench/atax.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/bicg/bicg.c -o polybench/bicg.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/cholesky/cholesky.c -o polybench/cholesky.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/doitgen/doitgen.c -o polybench/doitgen.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/gemm/gemm.c -o polybench/gemm.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/gemver/gemver.c -o polybench/gemver.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/gesummv/gesummv.c -o polybench/gesummv.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/mvt/mvt.c -o polybench/mvt.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/symm/symm.c -o polybench/symm.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/syr2k/syr2k.c -o polybench/syr2k.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/syrk/syrk.c -o polybench/syrk.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/trisolv/trisolv.c -o polybench/trisolv.tile
${pollycc} ${PCFLAG} linear-algebra/kernels/trmm/trmm.c -o polybench/trmm.tile

${pollycc} ${PCFLAG} linear-algebra/solvers/durbin/durbin.c -o polybench/durbin.tile
${pollycc} ${PCFLAG} linear-algebra/solvers/dynprog/dynprog.c -o polybench/dynprog.tile
${pollycc} ${PCFLAG} linear-algebra/solvers/gramschmidt/gramschmidt.c -o polybench/gramschmidt.tile
${pollycc} ${PCFLAG} linear-algebra/solvers/lu/lu.c -o polybench/lu.tile
${pollycc} ${PCFLAG} linear-algebra/solvers/ludcmp/ludcmp.c -o polybench/ludcmp.tile

${pollycc} ${PCFLAG} medley/floyd-warshall/floyd-warshall.c -o polybench/floyd-warshall.tile
${pollycc} ${PCFLAG} medley/reg_detect/reg_detect.c -o polybench/reg_detect.tile

${pollycc} ${PCFLAG} stencils/adi/adi.c -o polybench/adi.tile
${pollycc} ${PCFLAG} stencils/fdtd-2d/fdtd-2d.c -o polybench/fdtd-2d.tile
${pollycc} ${PCFLAG} stencils/fdtd-apml/fdtd-apml.c -o polybench/fdtd-apml.tile
${pollycc} ${PCFLAG} stencils/jacobi-1d-imper/jacobi-1d-imper.c -o polybench/jacobi-1d-imper.tile
${pollycc} ${PCFLAG} stencils/jacobi-2d-imper/jacobi-2d-imper.c -o polybench/jacobi-2d-imper.tile
${pollycc} ${PCFLAG} stencils/seidel-2d/seidel-2d.c -o polybench/seidel-2d.tile




${pollycc} ${PCFLAGOMP} datamining/correlation/correlation.c -o polybench/correlation.tile.parallel
${pollycc} ${PCFLAGOMP} datamining/covariance/covariance.c -o polybench/covariance.tile.parallel

${pollycc} ${PCFLAGOMP} linear-algebra/kernels/2mm/2mm.c -o polybench/2mm.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/2mm/ex11_41.c -o polybench/ex11_41.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/3mm/3mm.c -o polybench/3mm.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/atax/atax.c -o polybench/atax.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/bicg/bicg.c -o polybench/bicg.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/cholesky/cholesky.c -o polybench/cholesky.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/doitgen/doitgen.c -o polybench/doitgen.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/gemm/gemm.c -o polybench/gemm.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/gemver/gemver.c -o polybench/gemver.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/gesummv/gesummv.c -o polybench/gesummv.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/mvt/mvt.c -o polybench/mvt.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/symm/symm.c -o polybench/symm.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/syr2k/syr2k.c -o polybench/syr2k.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/syrk/syrk.c -o polybench/syrk.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/trisolv/trisolv.c -o polybench/trisolv.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/kernels/trmm/trmm.c -o polybench/trmm.tile.parallel

${pollycc} ${PCFLAGOMP} linear-algebra/solvers/durbin/durbin.c -o polybench/durbin.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/solvers/dynprog/dynprog.c -o polybench/dynprog.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/solvers/gramschmidt/gramschmidt.c -o polybench/gramschmidt.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/solvers/lu/lu.c -o polybench/lu.tile.parallel
${pollycc} ${PCFLAGOMP} linear-algebra/solvers/ludcmp/ludcmp.c -o polybench/ludcmp.tile.parallel

${pollycc} ${PCFLAGOMP} medley/floyd-warshall/floyd-warshall.c -o polybench/floyd-warshall.tile.parallel
${pollycc} ${PCFLAGOMP} medley/reg_detect/reg_detect.c -o polybench/reg_detect.tile.parallel

${pollycc} ${PCFLAGOMP} stencils/adi/adi.c -o polybench/adi.tile.parallel
${pollycc} ${PCFLAGOMP} stencils/fdtd-2d/fdtd-2d.c -o polybench/fdtd-2d.tile.parallel
${pollycc} ${PCFLAGOMP} stencils/fdtd-apml/fdtd-apml.c -o polybench/fdtd-apml.tile.parallel
${pollycc} ${PCFLAGOMP} stencils/jacobi-1d-imper/jacobi-1d-imper.c -o polybench/jacobi-1d-imper.tile.parallel
${pollycc} ${PCFLAGOMP} stencils/jacobi-2d-imper/jacobi-2d-imper.c -o polybench/jacobi-2d-imper.tile.parallel
${pollycc} ${PCFLAGOMP} stencils/seidel-2d/seidel-2d.c -o polybench/seidel-2d.tile.parallel


if [ ${BUILD_ARM} == 1 ]; then
tar -cf polybench.tar polybench
scp run.sh polybench.tar root@10.19.132.177:/root/cschen/
fi

