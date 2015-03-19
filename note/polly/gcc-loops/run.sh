#!/bin/bash -a
prog=gcc-loops

echo "time ./${prog}-novec"
sudo time ./${prog}-novec 1
echo "time ./${prog}-polly"
sudo time ./${prog}-polly 1
echo "time ./${prog}-pollyvec"
sudo time ./${prog}-pollyvec 1
echo "time ./${prog}-clangvec_polly"
sudo time ./${prog}-clangvec_polly 1
echo "time ./${prog}-clangvec"
sudo time ./${prog}-clangvec 1 

