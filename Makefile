export CROSS_COMPILE?=arm-linux-gnueabi-
export ARCH?=arm
export KBUILD_VERBOSE=0

KERNEL_DIR = linux-kernel
export build_base = $(CURDIR)/build/

export kernel_build := $(addprefix $(build_base), kernel)
busybox_build := $(addprefix $(build_base), busybox)
export BUILD_OUTPUT_BASE=$(CURDIR)/build/samples

.PHONY: all
all: kernel rootfs
	./qemu-kernel.sh

output_dirs = $(addprefix $(build_base), uboot busybox samples kernel)
$(output_dirs):
	mkdir -p  $@

$(kernel_build)/.config: $(CURDIR)/config/kernel/vexpress_defconfig
	cp ./config/kernel/vexpress_defconfig $(kernel_build)/.config

$(kernel_build)/.config.old: $(kernel_build) $(kernel_build)/.config
	make -C $(KERNEL_DIR) oldconfig KBUILD_OUTPUT=$(kernel_build)
	make -C $(kernel_build) modules_prepare

$(kernel_build)/vmlinux: $(kernel_build)/.config.old
	make -C $(kernel_build) uImage LOADADDR=0x80008000 -j2
	make -C $(kernel_build) dtbs

.PHONY: kernel
kernel: $(kernel_build)/vmlinux
	@echo successfully compile linux kernel

$(busybox_build)/busybox: $(busybox_build) $(CURDIR)/config/busybox/busybox_defconf
	cp $(CURDIR)/config/busybox/busybox_defconf $(busybox_build)/.config
	make -C busybox-1.28.0/ oldconfig KBUILD_OUTPUT=$(busybox_build)
	make -C $(busybox_build)
	make -C $(busybox_build) install

.PHONY: rootfs
rootfs: $(busybox_build)/busybox samples
	find $(CURDIR)/build/samples -name "*.ko" | xargs cp -v -t $(busybox_build)/_install
	fakeroot ./make-rootfs.sh $(busybox_build) $(CURDIR)/busybox-1.28.0

.PHONY: busybox
busybox: $(busybox_build)/busybox
	@echo successfully compile busybox

.PHONY: samples
samples:$(kernel_build)/.config.old
	make -C samples/compile-module
	make -C samples/slab

vexpress_boot:
	make -C u-boot-2016.09 vexpress_ca9x4_defconfig
#	make -C u-boot-2016.09 menuconfig
	make -C u-boot-2016.09

