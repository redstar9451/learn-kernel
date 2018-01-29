#!/bin/bash
qemu-system-arm -m 512 -M vexpress-a9 -kernel build/kernel/arch/arm/boot/zImage -s -serial stdio -append "root=/dev/mmcblk0 console=ttyAMA0" -sd a9rootfs.ext3 -dtb build/kernel/arch/arm/boot/dts/vexpress-v2p-ca9.dtb
