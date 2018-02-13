#!/bin/bash
image=$1
rootfs=$2
dtb=$3
qemu-system-arm -m 512 -M vexpress-a9 \
    -s -serial stdio \
    -kernel $image \
    -initrd $rootfs \
    -dtb $dtb \
    -append "root=/dev/mmcblk0 console=ttyAMA0" \
