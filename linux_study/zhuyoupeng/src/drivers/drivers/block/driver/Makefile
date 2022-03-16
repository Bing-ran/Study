#ubuntu的内核源码树，如果要编译在ubuntu中安装的模块就打开这2个
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build	

		
# 开发板的linux内核的源码树目录
KERN_DIR = /home/master/sambashare/linux_study/drivers/kernel

obj-m	+= blkdev.o

all:
	make -C $(KERN_DIR) M=`pwd` modules 
cp:
	cp blkdev.ko /home/master/sambashare/linux_study/drivers/rootfs/rootfs/root/ -f

.PHONY: clean	
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf app

