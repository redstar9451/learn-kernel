export CROSS_COMPILE=arm-linux-gnueabi-
export ARCH=arm
export KBUILD_VERBOSE=1

KERNEL_DIR=linux-kernel
KBUILD_OUTPUT_KERNEL=$(CURDIR)/build/kernel

vexpress_boot:
	make -C u-boot-2016.09 vexpress_ca9x4_defconfig
	make -C u-boot-2016.09 menuconfig
	make -C u-boot-2016.09
vexpress_kernel:
	-mkdir $(KBUILD_OUTPUT_KERNEL) -p
	cp ./config/kernel/vexpress_defconfig $(KBUILD_OUTPUT_KERNEL)/.config
#	make -C $(KERNEL_DIR) vexpress_defconfig KBUILD_OUTPUT=$(KBUILD_OUTPUT_KERNEL)
	make -C $(KERNEL_DIR) oldconfig KBUILD_OUTPUT=$(KBUILD_OUTPUT_KERNEL)
	make -C $(KERNEL_DIR) prepare KBUILD_OUTPUT=$(KBUILD_OUTPUT_KERNEL)
	make -C $(KBUILD_OUTPUT_KERNEL) uImage LOADADDR=0x80008000 -j2
	make -C $(KBUILD_OUTPUT_KERNEL) dtbs

.PHONY: busybox
busybox:
	make -C busybox/busybox-1.28.0/ defconfig
	make -C busybox/busybox-1.28.0/ 
	make -C busybox/busybox-1.28.0/ install
