#!/bin/bash
KBUILD_OUTPUT_BUSYBOX=$1
BUSYBOX_SRC=$2
cd $KBUILD_OUTPUT_BUSYBOX/_install
mkdir root home etc dev lib tmp mnt sys proc && mkdir ./usr/lib

mknod ./dev/console c 5 1
mknod ./dev/full c 1 7
mknod ./dev/kmem c 1 2
mknod ./dev/mem c 1 1
mknod ./dev/null c 1 3
mknod ./dev/port c 1 4
mknod ./dev/random c 1 8
mknod ./dev/urandom c 1 9
mknod ./dev/zero c 1 5
ls -l ./dev/

rm linuxrc
ln -s ./bin/busybox init

mknod ./dev/tty c 5 0
for i in `seq 0 9`; do
	mknod ./dev/tty$i c 4 $i
done

cp -ra $BUSYBOX_SRC/examples/bootfloppy/etc/* ./etc/
find . | cpio --quiet -H newc -o | gzip -9 -n > $KBUILD_OUTPUT_BUSYBOX/rootfs.cpio.gzip
