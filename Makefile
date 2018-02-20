export CROSS_COMPILE ?= arm-linux-gnueabi-
export ARCH ?= arm
export KBUILD_VERBOSE = 0

export build_root = $(CURDIR)/.build
kernel_src = $(CURDIR)/linux-kernel
# sameples need kernel_build, do not delete export keyword
export kernel_build := $(addprefix $(build_root)/, kernel)

busybox_build := $(addprefix $(build_root)/, busybox)
busybox_src := $(CURDIR)/busybox-1.28.0/

.PHONY: all
all: rootfs
	./scripts/qemu-kernel.sh \
		$(kernel_build)/arch/arm/boot/zImage \
		$(build_root)/rootfs.cpio.gzip \
		$(kernel_build)/arch/arm/boot/dts/vexpress-v2p-ca9.dtb

run:
	./scripts/qemu-kernel.sh \
		$(kernel_build)/arch/arm/boot/zImage \
		$(build_root)/rootfs.cpio.gzip \
		$(kernel_build)/arch/arm/boot/dts/vexpress-v2p-ca9.dtb


$(kernel_build)/.config: $(CURDIR)/config/kernel/vexpress_defconfig
	-mkdir -p $(kernel_build)
	cp ./config/kernel/vexpress_defconfig $(kernel_build)/.config
	make -C $(kernel_src) oldconfig KBUILD_OUTPUT=$(kernel_build)
	make -C $(kernel_build) modules_prepare

.PHONY: kernel
kernel: $(kernel_build)/.config
	make -C $(kernel_build) uImage LOADADDR=0x80008000 -j$(shell nproc)
	make -C $(kernel_build) dtbs

# make oldconfig even only busybox_src changed, I know that
.PHONY: busybox
busybox:
	-mkdir -p $(busybox_build)
	cp $(CURDIR)/config/busybox/busybox_defconf $(busybox_build)/.config
	make -C  $(busybox_src) oldconfig KBUILD_OUTPUT=$(busybox_build)
	make -C $(busybox_build)
	
.PHONY: rootfs
rootfs: busybox samples
	-rm -rf $(build_root)/rootfs
	-mkdir -p $(build_root)/rootfs
	make -C $(busybox_build) install CONFIG_PREFIX=$(build_root)/rootfs
	find $(build_root)/samples -name "*.ko" | xargs cp -v -t $(build_root)/rootfs
	fakeroot ./rootfs-example/make-rootfs.sh $(build_root) $(CURDIR)/rootfs-example/ $(build_root)/rootfs

.PHONY: samples
samples: kernel
	make -C samples

vexpress_boot:
	make -C u-boot-2016.09 vexpress_ca9x4_defconfig
#	make -C u-boot-2016.09 menuconfig
	make -C u-boot-2016.09

