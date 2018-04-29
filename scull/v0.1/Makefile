ifneq ($(KERNELRELEASE),)
	obj-m += scull.o
else
	KDIR := /usr/src/linux-headers-4.10.0-42-generic

default:
	$(MAKE) -C $(KDIR) M=`pwd`
endif