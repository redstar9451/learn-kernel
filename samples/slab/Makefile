#
#Makefile for the helloworld.c
#
obj-m := samples-slab.o
CURRENT_PATH := $(shell pwd)
LINUX_KERNEL := $(shell uname -r)
all:
	$(MAKE) -C $(KBUILD_OUTPUT_KERNEL) M=$(CURRENT_PATH) modules
clean:
	rm *.ko
	rm *.o

