#!/bin/bash
build_root=$1
etc_path=$2
rootfs=$3

cd $rootfs

mkdir root home etc dev lib tmp mnt sys proc && mkdir ./usr/lib
mkdir ./lib/modules/ -p
mv *.ko ./lib/modules
# in older kernel, mdev can not create console and null device
mknod ./dev/console c 5 1
mknod ./dev/null c 1 3
ls -l ./dev/

rm linuxrc
ln -s ./bin/busybox init

cp -ra $etc_path/etc/* ./etc/
find . | cpio --quiet -H newc -o | gzip -9 -n > $build_root/rootfs.cpio.gzip
