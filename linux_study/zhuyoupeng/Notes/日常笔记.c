字符串拷贝：
	memcpy：内存拷贝，按长度拷贝。可以开呗中间有 \0 的字符串。  缺陷：长度没弄好容易多拷贝（拷贝到不该拷贝的）

	strcpy ：	字符串拷贝，以 \0 结束。		缺点：在数组这种不带结束符的数据结构中，容易拷贝到莫名其妙的东西。
	strncpy：	字符串拷贝，以 \0 和 n 结束。	注意：如果拷贝n个字符之前无 \0 则拷贝 n个字符，如果有，则以 " " 占位。


i2c总线详解：
	时钟线： SCL高 有效
			SCL低 无效

	起始信号：
		SCL：高   
		SDA：高 --> 低

	起始信号：
		SCL：高   
		SDA：低 --> 高

	写时序：
		主设备	从地址+write flag -->	从设备
					<-- ACK 
					--> 内部寄存器地址

				write

	读时序：
		主设备	从地址+read flag -->		从设备
					<-- ACK 
					--> 内部寄存器地址
					<-- ACK
				从地址+read flag -->	
					<-- ACK

				read

	空闲状态：
		SCL高
		SDA高

i2c驱动分析：
	i2c_register_driver
		driver_register
			bus_add_driver
				driver_attach
					__driver_attach
						driver_match_device			内部调用设备端提供的match函数
						driver_probe_device			调用probe函数
							really_probe
								dev->bus->probe 	调用bus 中的probe函数 然后回去调用 driver中的probe函数
								即：
								i2c_device_probe
									driver->probe  	此处调用 driver中的probe函数

	探测驱动的address_list列表分析：
		i2c_register_driver
			i2c_for_each_dev
				__process_new_driver

i2c适配器驱动：
	i2c_add_adapter
		__i2c_add_numbered_adapter					设备树方式
			i2c_register_adapter
				device_register
					of_i2c_register_devices
						of_i2c_register_device

					i2c_scan_static_board_info


spi驱动分析：
	三部分： 
		spi设备驱动：
			spi设备驱动：	spi_driver
			spi设备：			spi_device
		spi核心：
			spi核心：			spi_core.c 
		spi控制器驱动：
			spi控制器驱动：	spi_master
		
struct spi_transfer		描写一个spi读写缓冲区
对于spi_master来说，一次数据传输的单位由spi_message结构体描述，多个spi_transfer会链接到spi_message的transfers链表，可以认为spi_message就是一次SPI数据交换的原子操作。spi_master接收了一个spi_message，其中的spi_transfer应该按顺序被发送，并且不能被其它spi_message打断。

spi_new_device
	spi_new_device


视频framebuffer框架：



mqtt
	


char *buf[] = {
	"aaa",
	"bbb",
	"ccc",
}

xxx((void *)buf);


xxx(void *p) 
{
	char *p = ((int *)p)[0];
}

Linux应用层与内核层交互的几种方式
	1、 系统调用 open、read、write...
	2、 proc虚拟文件系统
	3、 mmap
	4、 内核通过信号通知应用层
	5、 内核空间调用应用程序
	

拍摄帧率：24
慢动作： 播放12帧
快动作： 播放48帧


串口驱动：
	s3c24xx_serial_probe
		ourport = &s3c24xx_serial_ports[dev->id];		挂接上实际的操作函数
		
	driver：
		uart_register_driver
		//将实际的串口操作挂接到 driver.ops 中
		tty_set_operations(normal, &uart_ops);
			tty_register_driver
				cdev_init(&driver->cdev, &tty_fops);

	应用层函数调用关系
		open 
			tty_fops.open 
				uart_ops.open
					uart_startup(state, 0);
						uport->ops->startup(uport);

		write 
			tty_fops.write
				uart_ops.write
					uart_start
						__uart_start
							port->ops->start_tx(port);			port = tty->driver_data->uart_port



内核层 与中断 访问数据

用户空间没有udev/mdev 怎么创建节点（怎么获取主次设备号）
	1、 通过netlink socket通讯，在内核和用户空间之间传递信息。kobject_uevent
	2、 从 /sys/dev/xxx/major:minor/uevent 获取主次设备号以及name去创建节点
	
内核中获取主次设备号


linux系统驱动调试手段：
		1、 printk
		2、 查看OOP消息
		3、 利用strace
		4、 利用内核内置的hacking选项
		5、 利用ioctl方法
		6、 利用/proc虚拟文件系统
		7、 利用kgdb

1、 查看OOP消息
[   38.399485] pc : [<bf003018>]    lr : [<c00f76d8>]    psr: 60000013
[   38.928018] [<bf003000>] (chrdev_init+0x0/0x24 [module_test]) from [<c00d23bc>] (do_one_initcall+0x64/0x1b4)
[   38.968040] Code: e24cb004 e59f000c eb568041 e3a00000 (e5800000)

2、 返回编ko文件
00000000 <init_module>:
   0:	e1a0c00d 	mov	ip, sp 				// 把进入 init_module 之前的栈指针保存
   4:	e92dd800 	push	{fp, ip, lr, pc}
   8:	e24cb004 	sub	fp, ip, #4
   c:	e59f000c 	ldr	r0, [pc, #12]	; 20 <init_module+0x20>
  10:	ebfffffe 	bl	0 <printk>
  14:	e3a00000 	mov	r0, #0
  18:	e5800000 	str	r0, [r0]
  1c:	e89da800 	ldm	sp, {fp, sp, pc}
  20:	00000020 	andeq	r0, r0, r0, lsr #32

由上可知：
	出错在 chrdev_init的 (bf003018 - bf003000)/4 + 1 = 7 行出， 即 18:	e5800000 	str	r0, [r0] 处，与[   38.968040] Code: e24cb004 e59f000c eb568041 e3a00000 (e5800000)相同

注意汇编前三条：



中断：
	IRQ number
	HW interrupt ID
	irq domain 		（每一个中断控制器一个 domain）
	中断流控处理
		何时对中断控制器发出ack回应；
		对于电平中断触发的中断，mask_irq和unmask_irq的处理，
		有些中断控制器需要eoi回应，有些不用；


驱动注册中断服务例程，实际上就是创建与初始化一个irqaction，然后插入irq_desc的action链表。

当发生中断时，执行irq_desc的handle_irq回调函数，回调函数会遍历action中的handler函数，进行具体的中断处理。


IRQ domain支持三种映射方式
	linear map(线性映射)
		linear map：维护固定大小的表，索引是硬件中断号，如果硬件中断最大数量固定，并且数值不大，硬件中断号连续，可以选择线性映射；

	Radix tree map(树映射)
		Radix tree map：硬件中断号可能很大，可以选择树映射；

	no map(直接映射)
		no map：硬件中断号直接就是Linux的中断号；



device注册流程
	platform_device_register
		device_initialize
		platform_device_add
			device_add
				bus_add_device									// 将设备挂入总线的设备链表
			kobject_uevent(&dev->kobj, KOBJ_ADD);		// 创建dev/xxx节点
			bus_probe_device(dev);							// 调用match函数及probe函数
				device_initial_probe
					__device_attach
						__device_attach_driver
							driver_match_device				// 调用match
							driver_probe_device				// 调用probe
								really_probe(dev, drv);
									ret = dev->bus->probe(dev);	// 调用总线的probe函数

通用 Makefile 的编写
#开发板的内核源码树目录
KERNEL_DIR=...

obj-m += xxx.o 

all:
	make -C $(KERNEL_DIR) M=$(shell pwd) modules 

.PHONY: clean
clean:
	make -C $(KERNEL_DIR) M=$(shell pwd) modules clean



V4L2驱动框架
	1）分为4部分
		1、 字符设备驱动核心。	v4l2本身是一个字符设备，具有字符设备的所有特性，暴露给用户空间。
		2、 V4L2驱动核心。		主要是构建一个内核中标准视频设备驱动的框架，为视频操作提供统一的操作接口。
		3、 平台V4L2设备驱动。	在V4L2框架下，根据平台自身的特性实现的与平台相关的V4L2驱动部分，包括注册video_device和v4l2_dev。
		4、 具体的sensor驱动。	主要上电、提供工作时钟、视频图像裁剪、流IO开启等，实现各种设备控制方法供上层调用注册v4l2_subdev。

	2）V4L2的几个主要的结构体
		1、 v4l2_device：这个是整个输入设备的总结构体，可以认为它是整个V4L2框架的入口，充当驱动的管理者以及入口监护人。用于视频输入设备整体的管理，有多少输入设备就有多少个v4l2_device抽象（比如一个USB摄像头整体就可以看作是一个V4L2 device）。

		2、 v4l2_subdev：依附在v4l2_device之下，并表示一个v4l2设备的子设备，一个v4l2_devide下可以有多个v4l2_subdev，如硬件架构图，sensor当作一个v4l2_subdev。

		3、 vb2_queue：v4l2设备的缓存管理。
		4、 video_device：为用户空间提供设备节点，提供系统调用的相关操作(open、ioctl…)。

	3）主要文件
		1、 v4l2-dev.c 	// linux版本2视频捕捉接口，主要结构体 video_device 的注册
		2、 v4l2-common.c 	// 在linux操作系统体系采用低级别的操作一套设备 structures/vectors的通用视频接口。此文件将替换videodev.c的文件配备常规的内核分配。
		3、 v4l2-device.c 	// v4l2的设备支持。 注册v4l2_device
		4、 v4l2-ioctl.c 	// 处理v4l2的ioctl命令的一个通用的框架。
		5、 v4l2-subdev.c 	// v4l2子设备
		6、 v4l2-mem2mem.c // 内存到内存为linux的videobuf视频设备的框架。设备的辅助函数，使用其源和目的地videobuf缓冲区。
		7、 头文件linux/videodev2.h、media/v4l2-common.h、media/v4l2-device.h、media/v4l2-ioctl.h、media/v4l2-dev.h、media/v4l2-ioctl.h等。



数据结构分析：
		struct v4l2_device
			struct list_head subdevs;
				struct list_head list;   //用于挂入v4l2_device的subdevs链表
				const struct v4l2_subdev_ops *ops;  //操作集
					const struct v4l2_subdev_core_ops	*core;
					const struct v4l2_subdev_video_ops	*video;
					const struct v4l2_subdev_pad_ops	*pad;

			struct video_device 		//video_device结构体填充完，video_register_device 注册后会产生/dev/xxx节点
				struct cdev *cdev;
					struct file_operations v4l2_fops 
							↓		↓
							↓		↓
							↓		const struct v4l2_file_operations *fops;		// struct ↓video_device的fops成员是驱动程序设置的，但fops里的ioctl和poll成员可以不用驱动程序实现，内核提供的有实现好的函数
							↓
							↓
				const struct v4l2_ioctl_ops *ioctl_ops;

				struct vb2_queue *queue;	// 一般在v4l2_file_operations的open回调里设置并调用vb2_queue_init函数初始化vb2_queue
					const struct vb2_ops		*ops;
					const struct vb2_mem_ops	*mem_ops;	== vb2_vmalloc_memops/vb2_dma_sg_memops/vb2_dma_contig_memops
						void		*(*alloc)(...);
						int		(*mmap)(...);

					
v4l2设备读取数据的方式有两种，
	1、 一种是read方式
		就是通过read函数读取
	2、 一种是streaming方式
		streaming方式是在内核空间中维护一个缓存队列，然后将内存映射到用户空间，应用读取图像数据就是一个不断地出队列和入队列的过程