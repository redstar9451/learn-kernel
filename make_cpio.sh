#!/bin/bash
KBUILD_OUTPUT_BUSYBOX=$1
cd $KBUILD_OUTPUT_BUSYBOX/_install
mkdir root home etc dev lib tmp mnt sys proc && mkdir ./usr/lib
mknod ./dev/console c 5 1
mknod ./dev/zero c 1 5
mknod ./dev/null c 1 3
ls -l ./dev/
ln -s ./bin/busybox init
find . | cpio --quiet -H newc -o | gzip -9 -n > $KBUILD_OUTPUT_BUSYBOX/rootfs.cpio.gzip
