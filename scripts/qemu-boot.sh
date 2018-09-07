#!/bin/bash
image=$1
use_tap=`ifconfig|grep tap0`
if [ -n "$use_tap" ]; then
    sudo=sudo
    net="-net nic,vlan=0 -net tap,vlan=0,ifname=tap0,script=no,downscript=no"
fi

set -x
$sudo qemu-system-arm -m 64 -M vexpress-a9 \
    -s \
    -serial stdio \
    -kernel $image \
    $net
