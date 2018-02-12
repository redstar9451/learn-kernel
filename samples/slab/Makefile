
ifeq ($(build_base),)
	$(error variable build_base is null)
endif

# can not subsitute build_base for build, I don't know why. WTF!!!!!!!!
# 也许是因为进入内核目录后才展开的变量，而内核里已经定义了 build 变量，我不像再说 WTF 了. 2018/2/12
output ?= $(build_base)/samples/$(shell basename $(CURDIR))

# never use "module" as the obj-m value, or you will fail to insert module.ko,
# with the message "insmod: can't insert 'module.ko': invalid parameter"
# WTF!!!!!!!!!
obj-m := slab.o

slab-y := samples-slab.o

#ccflags-y := -I$(src)/include

all: $(output)/Makefile
	make -C $(kernel_build) M=$(output) src=$(CURDIR) modules

$(output):
	mkdir -p "$@"

$(output)/Makefile: $(output)
	touch "$@"

clean:
	make -C $(kernel_build) M=$(output) src=$(CURDIR) clean

