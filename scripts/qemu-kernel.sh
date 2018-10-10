#!/bin/bash
image=$1
rootfs=$2
dtb=$3
use_tap=`ifconfig|grep tap0`
if [ -n "$use_tap" ]; then
    sudo=sudo
    net="-net nic,vlan=0 -net tap,vlan=0,ifname=tap0,script=no,downscript=no"
fi

set -x
$sudo qemu-system-arm -m 512 -M vexpress-a9 \
    -smp 4 \
    -s -serial stdio \
    -kernel $image \
    -initrd $rootfs \
    -dtb $dtb \
    -append "loglevel=0 root=/dev/mmcblk0 console=ttyAMA0" \
    $net \
    $4
