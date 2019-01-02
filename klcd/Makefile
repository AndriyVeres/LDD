ifneq ($(KERNELRELEASE),)
	obj-m += klcd.o
else
	KDIR := /usr/src/linux-headers-4.14.34-v7+

default:
	$(MAKE) -C $(KDIR) M=`pwd`
endif
