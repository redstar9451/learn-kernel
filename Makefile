export CROSS_COMPILE?=arm-linux-gnueabi-
export ARCH?=arm
export KBUILD_VERBOSE=1

KERNEL_DIR=linux-kernel
KBUILD_OUTPUT_KERNEL=$(CURDIR)/build/kernel
KBUILD_OUTPUT_UBOOT=$(CURDIR)/build/uboot
KBUILD_OUTPUT_BUSYBOX=$(CURDIR)/build/busybox

vexpress_boot:
	make -C u-boot-2016.09 vexpress_ca9x4_defconfig
#	make -C u-boot-2016.09 menuconfig
	make -C u-boot-2016.09

vexpress_kernel:
	-mkdir $(KBUILD_OUTPUT_KERNEL) -p
	cp ./config/kernel/vexpress_defconfig $(KBUILD_OUTPUT_KERNEL)/.config
	make -C $(KERNEL_DIR) oldconfig KBUILD_OUTPUT=$(KBUILD_OUTPUT_KERNEL)
	make -C $(KBUILD_OUTPUT_KERNEL) prepare
	make -C $(KBUILD_OUTPUT_KERNEL) uImage LOADADDR=0x80008000 -j2
	make -C $(KBUILD_OUTPUT_KERNEL) dtbs

.PHONY: busybox
busybox:
	-mkdir $(KBUILD_OUTPUT_BUSYBOX) -p
	-rm -rf $(KBUILD_OUTPUT_BUSYBOX)/_install
	cp ./config/busybox/busybox_defconf $(KBUILD_OUTPUT_BUSYBOX)/.config
	make -C busybox-1.28.0/ oldconfig KBUILD_OUTPUT=$(KBUILD_OUTPUT_BUSYBOX)
	make -C $(KBUILD_OUTPUT_BUSYBOX)
	make -C $(KBUILD_OUTPUT_BUSYBOX) install
	fakeroot ./make-rootfs.sh $(KBUILD_OUTPUT_BUSYBOX) $(CURDIR)/busybox-1.28.0

