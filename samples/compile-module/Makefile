
BUILD_OUTPUT_BASE ?= /Volumes/Samsung_T5/learn-kernel/build/samples
BUILD_DIR ?= $(BUILD_OUTPUT_BASE)/$(shell basename $(CURDIR))
BUILD_DIR_MAKEFILE ?= $(BUILD_DIR)/Makefile

# never use "module" as the obj-m value, or you will fail to insert module.ko,
# with the message "insmod: can't insert 'module.ko': invalid parameter"
# WTF!!!!!!!!!
obj-m := compile-module.o

compile-module-y := main.o
compile-module-y += sub1/a.o
compile-module-y += sub1/sub1-1/b.o
compile-module-y += sub2/b.o

ccflags-y := -I$(src)/include
ccflags-y += -I$(src)/sub1/include

all: $(BUILD_DIR_MAKEFILE)
	make -C $(KBUILD_OUTPUT_KERNEL) M=$(BUILD_DIR) src=$(CURDIR) modules

$(BUILD_DIR):
	mkdir -p "$@"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)
	touch "$@"

clean:
	make -C $(KBUILD_OUTPUT_KERNEL) M=$(BUILD_DIR) src=$(CURDIR) clean

