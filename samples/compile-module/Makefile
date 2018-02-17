
ifeq ($(kernel_build),)
	$(error variable kernel_build is null)
endif

build_root ?= $(CURDIR)/build/
# can not subsitute build_root for build, I don't know why. WTF!!!!!!!!
# 也许是因为进入内核目录后才展开的变量，而内核里已经定义了 build 变量，我不想再说 WTF 了. 2018/2/12
output ?= $(build_root)/samples/$(shell basename $(CURDIR))

# never use "module" as the obj-m value, or you will fail to insert module.ko,
# with the message "insmod: can't insert 'module.ko': invalid parameter"
# WTF!!!!!!!!!

module := compile-module
obj-m := $(module).o

$(module)-y := main.o
$(module)-y += sub1/a.o
$(module)-y += sub1/sub1-1/b.o
$(module)-y += sub2/b.o

ccflags-y := -I$(src)/include
ccflags-y += -I$(src)/sub1/include

all:
	-mkdir -p $(output)
	-touch $(output)/Makefile
	make -C $(kernel_build) M=$(output) src=$(CURDIR) modules

clean:
	make -C $(kernel_build) M=$(output) src=$(CURDIR) clean

