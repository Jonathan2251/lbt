~/test/0618-polly/llvm_arm_build/bin/clang -O0 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor.c -o sor.O0
~/test/0618-polly/llvm_arm_build/bin/clang -O0 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor3.c -o sor3.O0

~/test/0618-polly/llvm_arm_build/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor.c -o sor.O3
~/test/0618-polly/llvm_arm_build/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor3.c -o sor3.O3

scp sor.O0 sor3.O0 sor.O3 sor3.O3 run-sor-arm.sh root@10.19.132.186:/root/cschen/

