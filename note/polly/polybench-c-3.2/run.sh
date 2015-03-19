#!/bin/bash

DIFF=0
#DIFF=1
ARM=0

TIME="time -f \"%E real, %U user, %S sys\""
#TIME=""

echo ""
echo "time ./correlation"
./correlation 2> result
echo "time ./correlation.tile"
./correlation.tile 2> tile.result
echo "time ./correlation.tile.parallel"
./correlation.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result;
  diff result tile.parallel.result;
fi

echo ""
echo "time ./covariance"
./covariance 2> result
echo "time ./covariance.tile"
./covariance.tile 2> tile.result
echo "time ./covariance.tile.parallel"
./covariance.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result;
  diff result tile.parallel.result;
fi

echo ""
echo "time ./2mm"
./2mm 2> result
echo "time ./2mm.tile"
./2mm.tile 2> tile.result
echo "time ./2mm.tile.parallel"
./2mm.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./3mm"
./3mm 2> result
echo "time ./3mm.tile"
./3mm.tile 2> tile.result
echo "time ./3mm.tile.parallel"
./3mm.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./atax"
./atax 2> result
echo "time ./atax.tile"
./atax.tile 2> tile.result
echo "time ./atax.tile.parallel"
./atax.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./bicg"
./bicg 2> result
echo "time ./bicg.tile"
./bicg.tile 2> tile.result
echo "time ./bicg.tile.parallel"
./bicg.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./cholesky"
./cholesky 2> result
echo "time ./cholesky.tile"
./cholesky.tile 2> tile.result
echo "time ./cholesky.tile.parallel"
./cholesky.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./doitgen"
./doitgen 2> result
echo "time ./doitgen.tile"
./doitgen.tile 2> tile.result
echo "time ./doitgen.tile.parallel"
./doitgen.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./gemm"
./gemm 2> result
echo "time ./gemm.tile"
./gemm.tile 2> tile.result
echo "time ./gemm.tile.parallel"
./gemm.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./gemver"
./gemver 2> result
echo "time ./gemver.tile"
./gemver.tile 2> tile.result
echo "time ./gemver.tile.parallel"
./gemver.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./gesummv"
./gesummv 2> result
echo "time ./gesummv.tile"
./gesummv.tile 2> tile.result
echo "time ./gesummv.tile.parallel"
./gesummv.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./mvt"
./mvt 2> result
echo "time ./mvt.tile"
./mvt.tile 2> tile.result
echo "time ./mvt.tile.parallel"
./mvt.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./symm"
./symm 2> result
echo "time ./symm.tile"
./symm.tile 2> tile.result
echo "time ./symm.tile.parallel"
./symm.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./syr2k"
./syr2k 2> result
echo "time ./syr2k.tile"
./syr2k.tile 2> tile.result
echo "time ./syr2k.tile.parallel"
./syr2k.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./syrk"
./syrk 2> result
echo "time ./syrk.tile"
./syrk.tile 2> tile.result
echo "time ./syrk.tile.parallel"
./syrk.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

if [ ${ARM} != 1 ]; then
# ARM build fail, so skip this item now
echo ""
echo "time ./trisolv"
./trisolv 2> result
echo "time ./trisolv.tile"
./trisolv.tile 2> tile.result
echo "time ./trisolv.tile.parallel"
./trisolv.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi
fi

echo ""
echo "time ./trmm"
./trmm 2> result
echo "time ./trmm.tile"
./trmm.tile 2> tile.result
echo "time ./trmm.tile.parallel"
./trmm.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi



echo ""
echo "time ./durbin"
./durbin 2> result
echo "time ./durbin.tile"
./durbin.tile 2> tile.result
echo "time ./durbin.tile.parallel"
./durbin.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./dynprog"
./dynprog 2> result
echo "time ./dynprog.tile"
./dynprog.tile 2> tile.result
echo "time ./dynprog.tile.parallel"
./dynprog.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./gramschmidt"
./gramschmidt 2> result
echo "time ./gramschmidt.tile"
./gramschmidt.tile 2> tile.result
echo "time ./gramschmidt.tile.parallel"
./gramschmidt.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./lu"
./lu 2> result
echo "time ./lu.tile"
./lu.tile 2> tile.result
echo "time ./lu.tile.parallel"
./lu.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./ludcmp"
./ludcmp 2> result
echo "time ./ludcmp.tile"
./ludcmp.tile 2> tile.result
echo "time ./ludcmp.tile.parallel"
./ludcmp.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi



echo ""
echo "time ./floyd-warshall"
./floyd-warshall 2> result
echo "time ./floyd-warshall.tile"
./floyd-warshall.tile 2> tile.result
echo "time ./floyd-warshall.tile.parallel"
./floyd-warshall.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./reg_detect"
./reg_detect 2> result
echo "time ./reg_detect.tile"
./reg_detect.tile 2> tile.result
echo "time ./reg_detect.tile.parallel"
./reg_detect.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi



echo ""
echo "time ./adi"
./adi 2> result
echo "time ./adi.tile"
./adi.tile 2> tile.result
echo "time ./adi.tile.parallel"
./adi.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./fdtd-2d"
./fdtd-2d 2> result
echo "time ./fdtd-2d.tile"
./fdtd-2d.tile 2> tile.result
echo "time ./fdtd-2d.tile.parallel"
./fdtd-2d.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./fdtd-apml"
./fdtd-apml 2> result
echo "time ./fdtd-apml.tile"
./fdtd-apml.tile 2> tile.result
echo "time ./fdtd-apml.tile.parallel"
./fdtd-apml.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./jacobi-1d-imper"
./jacobi-1d-imper 2> result
echo "time ./jacobi-1d-imper.tile"
./jacobi-1d-imper.tile 2> tile.result
echo "time ./jacobi-1d-imper.tile.parallel"
./jacobi-1d-imper.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./jacobi-2d-imper"
./jacobi-2d-imper 2> result
echo "time ./jacobi-2d-imper.tile"
./jacobi-2d-imper.tile 2> tile.result
echo "time ./jacobi-2d-imper.tile.parallel"
./jacobi-2d-imper.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

echo ""
echo "time ./seidel-2d"
./seidel-2d 2> result
echo "time ./seidel-2d.tile"
./seidel-2d.tile 2> tile.result
echo "time ./seidel-2d.tile.parallel"
./seidel-2d.tile.parallel 2> tile.parallel.result
if [ ${DIFF} == 1 ]; then
  diff result tile.result
  diff result tile.parallel.result;
fi

