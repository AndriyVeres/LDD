ifneq ($(KERNELRELEASE),)
	obj-m += hello.o
else
	KDIR := /usr/src/linux-headers-4.10.0-40-generic

default:
	$(MAKE) -C $(KDIR) M=`pwd`
endif