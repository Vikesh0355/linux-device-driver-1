obj-m := pcd_platform_driver_dt.o
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR = /home/vikesh/BBB_Workspace/Downloads/linux-5.10.168-ti-r72/
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make -C $(HOST_KERN_DIR) M=$(PWD) clean
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
copy-dtb:
	scp /home/vikesh/BBB_Workspace/Downloads/linux-5.10.168-ti-r72/arch/arm/boot/dts/am335x-boneblack.dtb debian@192.168.6.2:/home/debian/Vikesh/Linux_drivers
	
copy-drv:
	scp *.ko  debian@192.168.6.2:/home/debian/Vikesh/Linux_drivers

