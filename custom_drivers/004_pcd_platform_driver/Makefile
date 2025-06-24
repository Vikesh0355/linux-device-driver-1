obj-m := pcd_device_setup.o pcd_platform_driver.o

ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
KERN_DIR ?= ~/BBB_Workspace/Downloads/linux-5.10.168-ti-r72
HOST_KERN_DIR := /lib/modules/$(shell uname -r)/build/

.PHONY: all clean help host

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(HOST_KERN_DIR) M=$(PWD) clean
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean

help:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help

host:
	$(MAKE) -C $(HOST_KERN_DIR) M=$(PWD) modules
