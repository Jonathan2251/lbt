#!/usr/bin/env bash

# Reference: https://risc-v-getting-started-guide.readthedocs.io/en/latest/linux-qemu.html

sudo qemu-system-riscv64 -nographic -machine virt \
     -kernel linux/arch/riscv/boot/Image -append "root=/dev/vda ro console=ttyS0" \
     -drive file=busybox,format=raw,id=hd0 \
     -device virtio-blk-device,drive=hd0
