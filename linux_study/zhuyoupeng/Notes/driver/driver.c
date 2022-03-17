Linux驱动学习：
5.02.03 最简单的模块源码分析
	1）修饰符
		1、 __init: 本质上是个宏定义，在内核源代码中就有 #define __init xxx，作用是将 修饰的函数放到 .init.text段中去（本来默认情况下函数是放在.text段中）。整个内核中所有的这类函数都会被链接器链接放入 .init.text 段中，所以所有的内核模块的 init修饰的函数其实是被统一放在一起的。内核启动时统一会加载 .init.text段中的这些模块函数，加载完后就会把这个段给释放掉以节省内存。
		2、 __exit: 

		总结：前面加 下划线的表示是内核用的，加的越多内深入。

5.02.5 用开发板来调试模块
	1）设置bootcmd是开发板通过tftp下载自己建立的内核源码树编译得到的zImage
		set bootcmd 'tftp 0x30008000 zImage;bootm 0x30008000'

	2）设置bootargs是开发板从nfs去挂在rootfs（内核配置记得打开是能nfs形式的rootfs）
		setenv bootargs root=/dev/nfs rw nfsroot=192.168.204.10:/home/master/sambashare/linux_study/drivers/rootfs/rootfs,v3 ip=192.168.204.11:192.168.204.10:192.168.204.254:255.255.255.0::eth0:off init=/linuxrc console=ttySAC2,115200

5.2.6 字符设备驱动工作原理1
	1）系统整体工作原理
		1、 应用层-> API -> 设备驱动 -> 硬件
		2、 API: open、read、write、read、close等
		3、 驱动源码中提供真正的open、read、write、close等函数实体。

	2）file operations结构体
		1、 元素主要是函数指针，用来挂接实体函数地址
		2、 每个设备驱动都需要一个该结构体类型的变量
		3、 设备驱动向内核注册时提供该结构体类型的变量

	3）祖册字符设备驱动
		1、 为何要祖册驱动
		2、 谁去负责注册
		3、 向谁注册
		4、 注册函数从哪里来
		5、 祖册前怎样？注册后怎样？注册产生什么结果？

5.2.8 字符设备驱动代码实践1
	1）思路和框架
		1、 目的：给空板子添加驱动壳子
		2、 核心工作量：file_operations及其元素填充，注册驱动。

	2）如何动手写驱动代码
		1、 脑海里现有框架，知道自己要干嘛
		2、 细节代码不需要一个字一个字敲，可以看到内核中去寻找参考代码复制过来改
		3、 写下的所有代码必须心里明白，不能似懂非懂

	3）开始动手
		1、 先定义file_operations结构体变量
		2、 open和close函数原型确定、内容填充
		3、 注册驱动（注意主设备号的设定）

5.2.9 字符设备驱动代码实践2
	1）注册驱动
		1、 主设备号的选择， cat /proc/devices 中空的可用
		2、 返回值检测

	2）驱动测试
		1、 insmod并且查看设备注册的现象
		2、 rmmod并且查看设备注销的现象

	3）让内核自动分配主设备号
		1、 为什么要让内核自动分配
			1、自己设定比较麻烦，而且存在重复的风险，不安全。

		2、 如何实现
			1、 register_chrdev(0, "xxx", &file_operations)  第一个参数传 0

		3、 测试 

5.2.10 应用程序如何调用驱动
	1）驱动设备文件的创建
		1、 何为设备文件
		2、 设备文件的关键信息：设备号 = 主设备号 + 次设备号	，使用 ls -l 去查看设备文件，就可以得到这个设备文件对应的主次设备号
		3、 使用mknod创建设备文件：mknod /dev/xxx c 主设备号 次设备号

	2）写应用来测试驱动
		1、 还是原来的应用
		2、 open、write、read、close等
			注意：close --> relase

		3、 实验现象预测和验证

	3）总结：
		1、 整体流程梳理、注意分层
		2、 后续工作：添加读写接口

5.2.11 添加读写接口
	1）在驱动中添加
	2）在应用中添加
	3）测试
	4）应用和驱动之间的数据交换
		1、 copy_to_user		kernel --> user
		2、 copy_from_user	kerenl <-- user

5.2.12 读写接口实践
	1）完成write和read函数
		1、 copy_from_user 函数的返回值定义，和常规的有点不同。返回值如果成功则返回0，如果不成功则返回尚未复制的剩下的字节数。
		2、 copy_to_user

5.2.13 驱动中如何操控硬件
	1）化是哪个硬件
		1、 硬件的物理原理不变
		2、 硬件操作接口（寄存器）不变
		3、 硬件操作代码不变

	2）哪里不同了？
		1、 寄存器地址不同。原来是直接用物理地址，现在需要用改物理地址在内核虚拟地址空间相对应的虚拟地址。寄存器的物理地址是cpu设计时决定的，从datasheet中查找到的。
		2、 编程方法不同。裸机中习惯直接用函数指针操作寄存器地址，而kernel中习惯用封装号的io读写函数来操作寄存器，以实现最大程度可移植性。

	3）内核的虚拟地址映射方法
		1、 为什么需要虚拟地址映射
		2、 内核中有2套虚拟地址映射方法：动态和静态
		3、 静态映射方法的特点： 
			内核移植时以代码的形式硬编码，如果要更改必须改源代码后重新编译内核
			在内核启动时建立静态映射，到内核关机时销毁，中间一直有效
			对于移植好的内核，你用不用他都在那里

		4、 动态映射方法的特点
			驱动程序根据需要随时动态的建立映射、使用、销毁映射
			映射是短期临时的
	4）如何选择虚拟地址映射方法
		1、 2种映射并不排它，可以同时使用
		2、 静态映射类似于C语言中全局变量，动态映射方式类似于C语言中malloc堆内存
		3、 静态映射的好处是执行效率高，坏处是始终占用虚拟地址空间；动态映射的好处是按需使用虚拟地址空间，坏处是每次使用前后都需要去创建代码映射&销毁映射（还得学会使用那些内核函数的使用）。

5.2.14 静态映射操作LED
	1）关于静态映射要说
		1、 不同版本内核中静态映射表位置、文件名可能不同
		2、 不同SoC的静态映射表位置、文件名可能不同
		3、 所谓映射表其实就是头文件中的宏

	2）三星版本内核中的静态映射表
		1、 主映射表位于：arch\arm\plat-s5p\include\plat\map-s5p.h
			CPU在安排寄存器地址时不是随意乱序分布的，而是按照模块去区分的。每一个模块内部的很多歌寄存器的地址时连续的。所以内核在定义寄存器地址时都是先找到基地址，然后再用基地址 + 偏移量去访问。

			map-s5p.h中定义的就是要用到的几个模块的寄存器基地址。
			map-s5p.h总定义的是模块的寄存器基地址的虚拟地址。

		2、 虚拟地址基地址定义在：arch\arm\plat-samsung\include\plat\map-base.h
			#define S3C_ADDR_BASE	(0xFD000000)	// 三星移植时确定的静态映射基地址，表中的所有虚拟地址都是以这个地址 + 偏移量来指定的。

		3、 GPIO相关的主映射表：arch\arm\mach-s5pv210\include\mach\regs-gpio.h
			表中是GPIO的各个端口的基地址的定义。

		4、 GPIO的具体寄存器定义位于：arch\arm\mach-s5pv210\include\mach\gpio-bank.h

5.2.15 静态映射操作LED2
	1）参考裸机中的操作方法添加LED操作代码
		1、 宏定义
		2、 在init和exit函数中分别点亮和熄灭LED

	2）实践测试
		1、 insmod和rmmod时观测LED亮灭变化
		2、 打印出寄存器的值和静态映射表中的分析对比

	3）将代码移动到open和close函数中去

5.2.15 静态映射操作LED3
	1）添加驱动中的写函数
		1、 先定义好应用和驱动之间的控制接口，这个是由自己来定义的。譬如定义为：应用向驱动写“on”则驱动让LED亮，应用向驱动写“off：，驱动就让LED灭。
		2、 应用和驱动的接口定义做的尽量简单，譬如用1个字节来表示。譬如定义：应用写“1”表示亮，写“0”表示灭。

5.2.16 动态映射操作LED3
	1）申请GPIO口（向内核申请/报告需要映射的io内存资源）
		request_mem_region
		release_mem_region

	2）映射
		ioremap
		iounmap

5.3.1 注册字符设备驱动新接口1
	1）新接口与老接口
		1、 老接口：register_chrdev
		2、 新接口：register_chrdev_region/alloc_chrdev_region + cdev 
			register_chrdev_region:主动指定设备号
			alloc_chrdev_region：动态分配设别号
		3、 为什么需要新接口

	2）cdev结构体
		1、 结构体
		2、 相关函数：cdev_alloc、cdev_init、cdev_add、cdev_del

	3）设备号
		1、 主设备号和次设备号
		2、 dev_t 类型
		3、 MKDEV、MAJOR、MINOR三个宏

	4）编程实践
		1、 使用register_chrdev_region + cdev_init + cdev_add 进行字符设备驱动注册
		
5.3.2 注册字符设备驱动新接口2
	1）注册 200 0 1 2 3
		from MKDEV(200, 0)
		count 4

5.3.3 注册字符设备驱动新接口3
	1）使用alloc_chrdev_region自动分配设备号
		1、 register_chrdev_region是在事先知道主次、设备号时使用的；要先查看cat /proc/devices 去查看可用的。

		2、 更简单、更智能的方法是让内核给我们自动分配一个主设备号，使用alloc_chrdev_region就可以自动分配了。

		3、 自动分配的设别号，我们必须去知道他的主次设备号，否则后面没法去 mknod 创建他对应的设备文件。

	2）得到分配的主设备号和次设备号
		1、 使用MAJOR宏和MINOR宏从 dev_t 得到 major 和 minor
		2、 反过来使用MKDEV宏从 major 和 minor 得到 dev_t
		3、 使用这些宏的代码具有可移植性

	3）中途出错的倒影式错误处理方法
		1、 内核中很多函数中包含了很多歌操作，这些操作每一步都有可能出错，而且出错后后面的步骤就没必要进行下去了。

5.3.3 注册字符设备驱动新接口4
	1）使用 cdev_alloc 
		1、 cdev_alloc的编程实践
		2、 从内存角度体会cdev_alloc用于不用的差别
		3、 这就是非面向对象的语言和面向对象的代码
		4、 再次感叹C语言的博大精深。

	2）cdev_init的替代
		1、 cdev_init源码分析
		2、 不使用cdev_init时的编程
			cdev->owner = THIS_MODULE;
			cdev->ops = &st_fops;

		3、 为什么讲这个


程序中内存来源：
	全局变量 --> .data数据段
	局部变量 --> 栈
	malloc 	--> 堆

5.3.5 字符设备驱动注册代码分析1
	1）老接口分析
		register_chrdev --> __register_chrdev（此函数次设备号默认为0，不能指定）
			__register_chrdev_region
			cdev_alloc
			cdev->owner = fops->owner;
			cdev->ops = fops;

			cdev_add --> kobj_map

	2）新接口分析
		register_chrdev_region --> __register_chrdev_region
		alloc_chrdev_region    --> __register_chrdev_region


旧接口：
	register_chrdev {dev_t + cdev_init + cdev_add}	申请设备号，并注册
		cdev_init 《==》 
			cdev.ops = &ops;
			cdev.onwer = THIS_MODULE;


	register_chrdev_region + cdev_init + cdev_add 

5.3.7 自动创建字符设备驱动的设备文件
	1）问题描述
		1、整体流程回顾
		2、使用mknod创建设备文件的缺点
		3、能否自动生成和删除设备文件
		
	2）解决方案：udev（嵌入式中用的是 mdev）
		1、 什么事udev？应用层的一个程序
		2、 内核驱动和应用层udev之间有一套消息传输机制（netlink协议）
		3、 应用层启动udev，内核驱动中使用响应接口
		4、 驱动注册和注销时信息会被传给udev，由 udev 在应用层进行设备文件的创建和删除

	3）内核驱动设备类相关函数
		1、 class_create
		2、 device_create

	4）编程实践


5.3.8 设备类相关代码分析
	1）sys文件系统简介
		1、 sys文件系统的设计思想
		2、设备类的概念
		3、 /sys/class/xxx/中的文件的作用

	2）
		1、 class_create		-->  创建 /sys/class/xxx，（xxx是class_create的第二个参数）
		2、 device_create	-->	 创建 /sys/class/xxx/zzz	（创建xxx目录下的zzz）
			zz目录下：
				dev     存储主次设备号
				uevent	就是内核给udev传递的信息

	3）class_create函数分析
		class_create 
			__class_create
				__class_register
					kset_register
						kobject_uevent  --> 内核向udev发送uevent事件，然后就会创建 dev/xxx

	4）device_create函数分析
		device_create
			device_create_vargs
				kobject_set_name_vargs	--> 设置名字
					device_register		--> 向内核注册设备
						device_add
							kobject_add

							device_create_file
							device_create_sys_dev_entry
							devtmpfs_create_node
							device_add_class_symlinks
							device_add_attrs
							device_pm_add

							bus_add_device
							dpm_sysfs_add
							

static struct device_attribute uevent_attr =
	__ATTR(uevent, S_IRUGO | S_IWUSR, show_uevent, store_uevent);	

	show_dev  展示，cat --> 对应这个方法
	store_dev 存储，echo --> 对应这个方法


5.3.9 静态映射表的建立过程分析
	1）建立映射表的三个关键波分
		1、 映射表具体物理地址和虚拟地址的值相关的宏定义
		2、 映射表建立函数
			该函数负责由（1）中的映射表来建立Linux内核的页表映射关系。

			#ifdef CONFIG_MACH_SMDKC110
			MACHINE_START(SMDKC110, "SMDKC110")
			#elif CONFIG_MACH_SMDKV210
			MACHINE_START(SMDKV210, "SMDKV210")
			#endif
				/* Maintainer: Kukjin Kim <kgene.kim@samsung.com> */
				.phys_io	= S3C_PA_UART & 0xfff00000,
				.io_pg_offst	= (((u32)S3C_VA_UART) >> 18) & 0xfffc,
				.boot_params	= S5P_PA_SDRAM + 0x100,
				//.fixup		= smdkv210_fixup,
				.init_irq	= s5pv210_init_irq,
				.map_io		= smdkc110_map_io,
				.init_machine	= smdkc110_machine_init,
				.timer		= &s5p_systimer,
			MACHINE_END

		.map_io 实现页表映射

		smdkc110_map_io
			s5p_init_io
				iotable_init

		结论：经过分析，真正的内核移植时给定的静态映射表在 arch/arm/plat-s5p/cpu.c中的 s5p_iodesc，本质是一个结构体数组，数组中每一个元素就是一个映射，这个映射描述了一段物理地址到虚拟地址之间的映射。这个结构体数组所记录的几个映射关系被 iotable_init 所使用，该函数负责将这个结构体数组格式的表建立成MMU所能识别的页表映射关系，这样在开机后可以直接使用相对应的虚拟地址来访问对应的物理地址。
		
		3、 开机时调用映射表建立函数
			问题：开机时（kernel启动时）smdkc110_map_io怎么被调用的？
				start_kernel
					setup_arch
						setup_machine
							paging_init
								devicemaps_init
									if (mdesc->map_io)
										mdesc->map_io();


5.3.11 动态映射结构体方式操作寄存器
	1）问题描述
		1、 效仿真实驱动中，用结构体封装的方式来进行单词多寄存器的地址映射。来代替 5.2.17 节中讲的多次映射。
		
5.3.12 内核提供的读写寄存器接口
	1）前面访问寄存器的方式
		1、 行不行
		2、好不好

	2）内核提供的寄存器读写接口
		1、 writel和readl
			#define writeb(v,c)		({ __iowmb(); writeb_relaxed(v,c); })	写1个字节	byte
			#define writew(v,c)		({ __iowmb(); writew_relaxed(v,c); })	写2个字节	字
			#define writel(v,c)		({ __iowmb(); writel_relaxed(v,c); })	写4个字节	双字

		2、 iowrite32和ioread32

	3）代码实践

	4）分析总结

5.4.01 何为驱动框架
	1）驱动是谁写的
		1、 驱动开发工程师
		2、 内核维护者

	2）驱动编程写作要求
		1、 接口标准化
		2、 尽量降低驱动开发者难度

	3）到底什么是驱动框架
		1、 内核中驱动部分维护者针对每个种类的驱动设计一套成熟的、标准的、典型的驱动实现，然后把不同厂家的同类硬件驱动中相同的部分出呕出来自己实现好，再把不同部分流出来接口个具体的驱动开发工程师来实现，这就叫驱动框架。

		2、 内核维护在内核中设计了一些统一管控系统资源的体系，这些体系让内核能够对资源在各个驱动之间的使用统一协调和分配，保证整个内核的稳定健康运行。譬如：系统中所有的GPIO就属于系统资源，每个驱动模块如果要使用某个GPIO就要先调用特殊的接口先申请，申请到后使用，使用完后释放。又譬如，中断号也是一种资源，驱动在使用前也必须去申请。这也是驱动框架的组成部分。

		3、 一些特定的接口函数、一些特定的数据结构，这些事驱动框架的直接表现。

5.4.02 内核驱动框架中LED的基本情况
	1）相关文件
		1、 drivers/leds目录		这个目录就是驱动框架规定的LED这种硬件的驱动应该待得地方。
		2、 led-class.c和led-core.c 	这两个文件属于LED驱动框架的第一部分，这两个文件是内核开发者提供的，他们描述的是内核中所有厂家的不同LED硬件的相同部分的逻辑。
		3、 leds-xxx.c  			这个文件是LED驱动框架的第二部分，是由不同厂商的驱动工程师编写添加的，厂商驱动工程师结合自己公式的硬件的不同情况来对LED进行操作，使用第一部分提供的接口来和驱动框架进行交互，最终实现驱动的功能。

	2）九鼎移植的内核中led驱动
		1、 九鼎实际未使用内核推荐的led驱动框架
		2、 drivers/char/led/x210-led.c 

	3）案例分析驱动框架的使用
		1、 以led-s3c24xx.c为例。 led-s3c24xx.c中通过调用 led_classdev_register 来完成我们的LED驱动注册，而 led_classdev_register 是在 drivers/leds/led-class.c 中定义的。所以其实SoC厂商的驱动工程师是调用内核开发者在驱动框架中提供的接口实现自己的驱动的。

		2、 驱动框架关键点就是：分清楚内核开发者提供了什么，驱动开发者自己要提供什么？

	4）典型的驱动开发行业现状
		1、 内核开发者对驱动框架进行开发和维护、升级。 led-class.c、led-core.c
		2、 SoC厂商的驱动工程师对设备驱动源码进行编写、调试，提供参考版本。 led-s3c24xx.c
		3、 做产品的厂商的驱动工程师从SoC厂商提供的驱动源码为基础，来做移植和调试。


5.4.03 初步分析led驱动框架源码
	1）涉及到的文件
		1、 led-core.c
		2、 led-class.c

	2）subsys_initcall
		1、 经过基本分析，发现LED驱动框架中内核开发者实现的部分主要是led-class.c
		2、 我们发现 led-class.c 就是一个内核模块，对led-class.c分析应该从下往上，尊从对模块的基本分析方法。
		3、 为什么LED驱动框架中内核开发者实现的部分要实现成一个模块？因为内核开发者希望这个驱动框架是可以被装载/卸载的。这样当我们内核使用者不需要这个驱动框架时可以完全去掉，需要时可以随时加上。
		4、 subsys_initcall是一个宏，定义在 include/linux/init.h中。经过对这个宏进行展开，发现这个宏功能是：将其声明的函数放到一个特定的段：.initcall4.init
			subsys_initcall 	
				__define_initcall("4s",fn,4s)

		5、 分析module_init宏
			module_init
				__initcall
					device_initcall 
						__define_initcall("6",fn,6)

		6、 内核在启动过程中需要顺序的做很多事，内核如何实现按照先后顺序去做很多初始化操作。内核解决方案就是给内核启动时要调用的所有函数归类，然后每个类按照一定的次序去调用执行。这些分类名就叫：.initcalln.init。n的值从1到8.内核开发者在编写内核代码时只要将函数设置合适的级别，这些函数会被链接的时候放入特定的段，内核启动时按照段的顺序去一次执行各个段。
		7、 经过分析可以看出，subsys_initcall和module_init的作用是一样的，只不过牵着所声明的函数要比后者在内核启动时的执行顺序更早。

	3）led_class_attrs
		1、 什么事attribute，对应将来/sys/class/leds/目录里的内容，一般是文件和文件夹。这些文件其实就是sysfs开放给应用层的一些操作接口（非常类似于/dev/目录下的那些设备文件）

		2、 attribute有什么用
			作用是让应用程序可以通过/sys/class/leds/目录下面的属性文件来操作驱动进而操作硬件设备。

		3、 attribute其实是另一条驱动实现的路线，

	4）led_classdev_register
		led_classdev_register
			device_create

		1、 分析可知：led_classdev_register这个函数其实就是区创建一个属于leds这个类的一个设备。其实就是去注册一个设备。所以这个函数其实就是led驱动框架中内核开发者提供给SoC厂家开发者的一个注册接口。

		2、 当我们使用led驱动框架去编写驱动的时候，这个led_classdev_register函数的作用类似于我们之前使用file_operations方式去注册字符设备驱动时的 register_chrdev 函数。

5.4.5 在内核中添加或去除某个驱动
	1）去穿九鼎移植的LED驱动

	2）添加led驱动框架支持

	3）sysfs中内容分析

	4）后续展望：完成led-x210.c

5.4.7 基于驱动框架写led驱动1
	1）分析
		1、 参考哪里？ 	drivers/leds/leds-s3c24xx.c
		2、 关键点： 		led_classdev_register

	2）手动写led驱动模块


5.4.7 基于驱动框架写led驱动2
	1）代码实践
		1、 调试
		2、 分析

		通过分析看出：
			1、 我们写的驱动确实工作了，被加载了，/sys/class/leds/目录下确实多出来了一个表示设备的文件夹。文件夹里面有相应的操控led硬件的2个属性brightness和max_brightness
			2、 led-class.c中brightness方法有一个show方法和store方法，这两个方法对应用户在/sys/class/leds/mtled/brightness目录下直接去读写这个文件时实际执行的代码。当我们show brightness时，实际就会执行led_brightness_show函数，
			当我们echo 1 > brightness时，实际就会执行 led_brightness_store函数。
			3、 show方法实际要做的就是读取LED硬件信息，然后把硬件信息返回给我们即可。所以show方法和store方法必须要回去操控硬件。但是led-class.c文件有属于驱动框架中的文件，它本身无法直接读取具体硬件，因此在show和store方法中使用函数指针的方式调用了 struct led_classdev结构体中的相应的读取/写入硬件信息的方法。
			4、 struct led_classdev结构体中的实际用来读写硬件信息的函数，就是我们自己写的驱动文件 leds-s5pv210.c 中要提供的。

	2）

5.4.7 基于驱动框架写led驱动3
	1）在驱动层中将4个LED分开
		1、 好处。驱动层实现对各个LED设备的独立访问，并向应用层展示出4个操作接口led1、led2、led3、led4，这样应用层可以完全按照自己的需求对LED进行控制。

		驱动设计理念：不要对最终需求功能进行假定，而应该只是直接的对硬件的操作。有一个概念就是：机制和策略的问题。在硬件操作上驱动只应该提供机制特不是提供策略。策略由应用层程序来做。

		2、 如何实现

	2）和leds-s3c24xx.c的不同

	3）gpiolib的引入
		1、 一个事实：很多硬件都要用到GPIO、GPIO会复用
		2、 如果同一个GPIO被2个驱动同时控制了，就会出现bug
		3、 内核提供gpiolib来统一管理系统中所有GPIO
		4、 gpiolib本身属于驱动框架的一部分

5.4.9 Linux内核的gpiolib学习1
	1）gpiolib学习重点
		1、 gpiolib的建立过程
		2、 gpiolib的使用方法：申请、使用、释放
		3、 gpiolib的架构：涉及哪些目录的哪些文件

	2）gpiolib的学习方法
		1、 以一条主线进去，坚持主线
		2、 中途遇到杂碎知识，触底搞定之，然后继续主线
		3、 随时做笔记以加深理解和记忆
		4、 学习中注意架构思想，提升自己大脑的空间复杂度

	3）主线1：gpiolib的建立
		1、 找到目标函数
			mach-smdkc110.c 
				smdkc110_map_io
					s5pv210_gpiolib_init	gpiolib初始化函数	（gpiolib.c）


5.4.9 Linux内核的gpiolib学习2
	1）struct s3c_gpio_chip
		1、 这个结构体是一个GPIO端口的抽象。
		2、 端口和IO口是两个概念。S5PV210有很多歌IO口（160个左右），这些IO口首先被分成N个端口（port group），然后每个端口又包含了M个IO口。譬如：GPA0是一个端口，里面包含了8个IO口，我们一般记做：GPA0_0（或者GPA0.0）、GPA0_1、
		3、 内核中位每个GPIO分配了一个编号，编号是一个数字（譬如一共有160个IO时编号就可以从1到160连续分布），编号可以让程序员很方便的去识别每一个GPIO。

	2）s5pv210_gpio_4bit
		1、 这个东西是一个结构体数组，数组中包含了很多歌 struct s3c_gpio_chip 类型的变量。


5.4.9 Linux内核的gpiolib学习3
	1）S5PV210_GPA0宏
		1、 这个宏的返回值是GPIAO端口某一个IO口的编号值，传参就是我们这个IO在GPA0端口中的局部编号。

	2）samsung_gpiolib_add_4bit_chips 这个函数才是及具体进行gpiolib的注册的。这个函数接收的参数是我们当前文件中定义好的结构体数组s5pv210_gpio_4bit （其实2个参数分别是数组名和元素个数），这个数组中其实就包含了当前系统中所有的IO端口的信息（这些信息包含：端口名、端口中所有的GPIO的编号、端口操作寄存器组的虚拟地址的基地址、端口的IO口数量，端口上下拉等模式的配置函数、端口中的IO口换算其对应的终端号函数）。


5.4.9 Linux内核的gpiolib学习4
	1）几个问题
		1、 那个目录的那个文件
		2、 函数名中的为什么有个4bit：三星的CPU总2440的CON寄存器是2bit对应一个IO口，而6410和210以及之后的系列中CON寄存器是4bit对应1个 IO口。所以gpiolib在操作2440和210的CON寄存器时是不同的。

	2）函数调用关系
		samsung_gpiolib_add_4bit_chips
			samsung_gpiolib_add_4bit
			s3c_gpiolib_add
经过分析，发现 samsung_gpiolib_add_4bit 这个函数内部其实并没有做gpiolib的注册工作，而是还在做填充，填充的是每一个GPIO被设置成输入模式/输出模式的操作方法。


5.4.13 linux内核的gpiolib学习5
	1）s3c_gpiolib_add
		1、 首先检测并完善chip的 direction_input/direction_output/set/get这4个方法
		2、 然后调用 gpiochip_add 方法进行真正的注册操作。其实这个注册就是将我们的封装了一个GPIO端口的所有信息的chip结构体变量挂接到内核gpiolib模块定义的一个gpio_desc数组中的某一个格子中。

	2）从驱动框架的角度再来分析下gpiolib
		1、 之前的分析已经告一段落，截止目前已经清楚了gpiolib的建立过程。但是这只是整个gpiolib建立的一部分，是厂商驱动工程师负责的那一部分；还有一部分是内核开发者提供的驱动框架的那一部分，就是我们后面去分析的第2条主线。

		2、 drivers/gpio/gpiolib.c这个文件中所有的函数构成了我们第2部分，也就是内核开发者写的gpiolib框架部分。这个文件中提供的函数主要有一下部分：
			gpiochip_add：	是框架开出来的接口，给厂商的驱动工程师用，用于向内核注册我们的gpiolib部分。
			gpio_request：	是框架开出的接口，给使用gpiolib来编写自己的驱动的驱动工程师用的，驱动中要想使用某一个gpio，就必须先调用 				gpio_request 接口来向内核gpiolib部分申请，得到允许后才可以去使用这个gpio。
			gpio_free：		对应gpio_request，用来释放使用完的gpio

			gpio_request_one/gpio_request_array： 这两个是gpio_request的变种。

			gpiochip_is_requested：	判断gpio是否被申请。
			gpio_direction_input/gpio_direction_output：		这两个接口使用来设置GPIO位输入/输出模式，注意：该方法中没有具体的对硬件的操作，而是通过函数指针调用厂商写的具体操作。

以上的接口属于一类，这些都是给写其他驱动并且用到了gpiolib的人使用的，剩下的还有一类函数，这类函数是gpiolib内部自己的一些功能实现的代码。


5.4.13 linux内核的gpiolib学习6
	postcore_initcall(gpiolib_sysfs_init);
		gpiolib_sysfs_init
			gpiochip_export
				sysfs_create_group		--> 各种attribute都在这里产生


目录文件结构：
	mach-smdkc110.c/gpiolib.c 							s5pv210_gpiolib_init
	mach-s5pv210/include/mach/gpio.h 					#define S5PV210_GPA0(_nr)	(S5PV210_GPIO_A0_START + (_nr))
	arch/arm/mach-s5pv210/gpiolib.c 					samsung_gpiolib_add_4bit_chips(s5pv210_gpio_4bit, nr_chips);
	
	arch/arm/plat-samsung/gpiolib.c  					里面是210/6410这种4bit CON寄存器类型的操作方法
	arch/arm/plat-samsung/gpio.c  						里面是24xx这种2bit CON寄存器类型的操作方法

	drivers/gpio/gpiolib.c  
								里面是内核开发者写的gpiolib的
#define S5PV210_GPIO_NEXT(__gpio) \
	((0) + (8) + 0 + 1)	

5.4.16 使用gpiolib完成led驱动
	1）流程分析
		1、 第一步：gpio_request 申请要使用的gpio
		2、 第二部：gpio_direction_input/gpio_direction_output  设置gpio的工作模式
		3、 第三部：设置输出值	：gpio_set_value，获取端口值：gpio_get_value

	2）代码实践
		1、 第一步实在led1上编写代码测试通过
		2、 第二部是在led2、led3上扩展
		3、 学习Linux中查看gpio使用情况的方法

		内核中提供了虚拟文件系统dbugfs，里面有一个gpio文件，提供了gpio的使用信息
		使用方法：mount -t debugfs debugfs /tmp，然后cat /tmp/gpio即可得到gpio的所有信息，使用完后 umount /tmp卸载debugfs即可。


5.4.17 将驱动添加到内核中
	1）驱动的存在形式
		1、 野生，优势方便调试开发，所以在开发阶段都是这种
		2、 家养，优势可以再内核配置时 make menuconfig 决定内核怎么编译，方便集成。

	2）驱动开发的一般步骤
		1、 以模块的形式在外部编写、调试
		2、 将调好的驱动代码集成到kernel中

	3）实践
		1、 关键点：Kconfig、Makefile、make menuconfig
		2、 关键步骤：
			第一步：将携号的驱动源文件放入内核源码中正确的目录下
			第二部：在Makefile中添加响应的依赖
			第三部：在Kconfig中添加响应的配置项
			第四部：make menuconfig


5.05.1 Linux设备驱动模型简介
	1）什么事设备驱动模型
		1、 类class、总线bus、设备device、驱动driver
		2、 kobject和对象生命周期
		3、 sysfs
		4、 udev

	2）为什么需要设备驱动模型
		1、 早期内核（2.4之前）没有统一的设备驱动模型，但照样可以用
		2、 2.6版本中正式引入设备驱动模型，目的是在设备越来越多，工号要求等新特性要求的情况下让驱动体系更易用、更优秀。
		3、 设备驱动模型负责统一实现和维护一些特性，诸如：电源管理、热插拔、对象生命周期、用户空间和驱动空间的交互等基础设施。
		4、 设备驱动模型目的是简化驱动程序编写，但是客观上设备驱动模型本身设计和实现很复杂。

	3）驱动开发的2个点
		1、 驱动源码本身编写、调试。重点在于对硬件的了解。
		2、 驱动什么时候安装、驱动中的函数什么时候调用。跟硬件无关，完全和设备驱动模型有关。


5.5.2 设备驱动模型的底层架构
	1）kobject
		1、 定义在 linux/kobject.h 中
		2、 各种对象最基本单元，提供一些公用型服务如：对象引用计数、维护对象链表、对象上锁、对用户空间的表示
		3、 设备驱动模型中的各种对象其内部都包含一个kobject
		4、 地位相当于面向对象架构中的总基类

	2）kobj_type
		1、 很多书中简称为 ktype，每一个kobject都需要绑定一个ktype来体用响应功能
		2、 关键点1：sysfs_ops，提供该对象在sysfs中的操作方法（show和store）
		3、 关键点2：attribute，提供在sysfs中以文件形式存在的属性，其实就是应用接口

	3）kset
		1、 kset的主要作用是做顶层 kobject的容器类
		2、 kset的主要目的是将各个kobject（代表着各个对象）组织出目录层次架构
		3、 可以认为kset就是为了在sysfs中弄出目录，从而让设备驱动模型中的多个对象能够有层次有逻辑性的组织在一起。

5.5.3 总线式设备组织方式
	1）总线
		1、 物理上的真实总线及其作用（英文bus）
		2、 驱动框架中的总线式设计
		3、 bus_type结构体，关键是match函数和uevent函数

	2）设备
		1、 struct device是硬件设备在内核驱动框架中的抽象
		2、 device_register用于向内核驱动框架注册一个设备
		3、 通常device不会单独使用，而是被包含在一个具体结构体中，如 struct usb_device

	3）驱动
		1、 struct device_driver是驱动程序在内核驱动架构中的抽象
		2、 关键元素1：name，驱动程序的名字，很重要，经常用来作为驱动程序和设备的匹配依据
		3、 关键元素2：probe，驱动程序的探测函数，用来检测一个设备是否可以被驱动所管理

	4）类
		1、 相关结构体：struct class 和 struct class_device
		2、 udev的使用离不开class
		3、 class的真正意义在于作为同属于一个class的多个设备的容器。也就是说，class是一种人造概念，目的就是为了对各种设备进行分类管理。当然，class在分类的同事还对每个类贴上了一些“标签”，这也是设备驱动模型为我们写驱动提供的基础设施。

	5）总结
		1、 模型思想很重要，其实就是面向对象的思想。
		2、 全是结构体套结构体，地基本功（语言功底和大脑复杂度）要求很高。

5.5.4 platform平台工作总线工作原理1
	1）何为平台总线（扩展到cpu 32位地址总线空间中的哪一类设备（很多），用platform来管理）。
		1、 相对usb、pci、i2c等物理总线来说，platform总线式虚拟的、抽象出来的。
		2、 回顾裸机中讲的，CPU与外部通信的2中方法：地址总线式链接和专用接口式链接。平台中线对应地址总线式连接设备，也就是SoC内部集成的各种内部外设。
		3、 思考：为什么要有平台总线？进一步思考：为什么要有总线的概念？ （管理方便设备与驱动）

	2）平台总线下管理的2员大将
		1、 platform工作体系都定义在 drivers/base/platform.c 中
		2、 两个结构体：platform_device 和 platform_driver
		2、 两个接口函数：platform_device_register 和 platform_driver_register

struct platform_device {
	const char	* name;				// device 名字
	int		id;
	struct device	dev;			// 通用 device 结构体
	u32		num_resources;
	struct resource	* resource;		// 设备使用到的资源数组的首地址

	const struct platform_device_id	*id_entry;	// 设备id表，很多相似的设备

	/* arch specific additions */
	struct pdev_archdata	archdata;		// 自留地，用来提供扩展性的
};

struct platform_driver {
	int (*probe)(struct platform_device *);			// 驱动探测函数
	int (*remove)(struct platform_device *);		// 删除/去掉一个设备（ 卸载）
	void (*shutdown)(struct platform_device *);		// 关闭一个设备
	int (*suspend)(struct platform_device *, pm_message_t state);	// 挂起一个设备
	int (*resume)(struct platform_device *);		// 恢复一个设备
	struct device_driver driver;					// 通用 device_driver 结构体
	const struct platform_device_id *id_table;		// 设备id表，很多相似的设备
};


5.5.5 platform平台总线工作原理2
	1）平台总线体系的工作流程
		1、 第一步：系统启动时在bus系统中注册platform
		2、 第二部：内核移植的人负责提供platform_device		（移植的人提供）
		3、 第三部：写驱动的人负责提供platform_driver		（驱动编写人员提供）
		4、 第四部：platform的match函数发现driver和device匹配后，调用driver的probe函数来完成驱动的初始化和安装，然后设备就工作起来了。

	2）代码分析：platform本身注册
		1、 每种总线（不光是platform、usb、i2c那些也是）都会带一个match方法，match方法用来对总线下的device和driver进行匹配。理论上每种总线的匹配算法是不同的，但是实际上一般都是看name的。
		2、 platform_match函数就是平台总线的匹配方法。该函数的工作方法是：如歌id_table就说明驱动可能支持多个设备，所以这时候要去对比id_table中所有的name，只要找到一个相同的就匹配上了不在找了，如果找完id_table都还没找到就说明没匹配上；如果没有id_table或者没匹配上，那就直接对比device和driver的name，如果匹配上了，如果还没撇皮上那就匹配失败了。


/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })

container_of(&a, a_t, a)


offsetof(type, member))  --> (size_t)(&((type *)0)->member)

解析：先将0 强制转换为type类型的结构体指针然后在指向member，最后取地址，取得的地址就是member相对0的偏移量，也就是member在type结构体中的偏移量



container_of(ptr, type, tr)
	const typeof(((type *)0)->tr) * __ptr = (ptr)
	(type *)(char *) __ptr - (size_t)(&((type *)0)->tr)


5.5.6 platform平台总线工作原理3
	1）以leds-s3c24xx.c为例来分析platform设备和驱动的注册过程
		1、 platform_driver_register
		2、 platform_device_register

	2）platdata/resource怎么玩
		1、 platdata其实就是设备注册时提供的设备有关的一些数据（譬如设备对应的gpio、是用到的中断号、设备名称...）
		2、 这些数据在设备和驱动match之后，会由设备方转给驱动方。驱动拿到这些数据后使用这些数据，通过这些数据得知设备的具体信息，然后来操作设备。
		3、 这样做的好处是：驱动源码中不携带数据，只负责算法（对硬件的操作方法）。现代的驱动设计理念是算法与数据分离。

	3）match函数的调用轨迹

	4）probe函数的功能和意义

5.5.7 平台总线实践环节1
	1）回顾
	2）现初步改造添加platform_driver
		1、 第1步：先修改原来的代码到只有led1
		2、 第2步：

	3）实验现象分析

5.5.8 平台总线实践环节2
	1）检查mach-x210.c中是否有led相关的platform_device
	2）参考mach-mini2440.c中添加led的platform_device定义
	3）测试只有platform_device没有platform_driver时是怎样的

5.5.8 平台总线实践环节3
	1）测试platform_device和platform_driver相遇会怎样
	2）probe函数
		1、 


5.6.1 板载蜂鸣器测试
	
5.6.2 misc类设备介绍
	1）何为misc
		1、 中文名：杂项设备
		2、 /sys/class/misc
		3、 典型的字符设备
		4、 有一套驱动框架，内核实现一部分（misc.c），驱动实现一部分（x210-buzzer.c）。
		5、 misc是对原始的字符设备注册接口的一个类层次的封装，很多典型字符设备都可以归类到misc类中，使用misc驱动框架来管理。

	2）misc类设备驱动框架
		1、 内核开发者实现部分，关键点有2个：一个是类的创建，另一个是开放给驱动懂开发者的接口。
		2、 具体设备驱动工程师的部分。

	3）本部分学习方法
		1、 蜂鸣器驱动源码已有，以分析为主
		2、 复习并验证前面讲的驱动框架的思维
		3、 有余力的不妨开始注意一些细节

5.6.3 misc驱动框架源码分析
	1）misc源码框架基础
		1、 misc源码框架本省也是一个模块，内核启动时自动加载
		2、 源码框架的主要工作：注册misc类，使用老接口注册字符设备驱动（主设备号10），开放device注册的接口misc_register给驱动工程师

	2）misc类设备的注册
		1、 驱动工程师需要借助misc来加载自己的驱动时，只需要调用misc_register接口注册自己的设备即可，其余不用管。
		2、 misc_list链表的作用。简单来说 内核定义了一个 misc_list 链表用来记录所有内核中注册了的杂散类设备。当我们向内核注册一个misc类设备时，内核就会向misc_list中insert一个节点。

		如果misc的次设备号是17，则自动分配。

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static LIST_HEAD(misc_list);

==> static struct list_head misc_list = { &(misc_list), &(misc_list) }


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

		3、 主设备号和次设备号的作用和区分
			1、 主设备号：区分一类设备 --> 类 
			2、 次设备号：区分个体

	3）open函数分析


5.6.4 蜂鸣器驱动源码分析1
	
5.6.5 蜂鸣器驱动源码分析2
	1）open函数分析
	2）misc在proc下的展现
	3）内核互斥锁
		1、 何为互斥锁
		2、 定义：DEFINE_MUTEX
		3、 上锁：mutex_lock 和解锁：mutex_unlock
		4、 内核防止竞争状态的手段：原子访问、自旋锁、互斥锁、信号量
		5、 原子访问主要用来做计数、自旋锁后面会详细讲、互斥锁和信号量很相似（其实就是计数值为1的信号量），互斥锁的出现比信号量晚，实现上比信号量优秀，尽量使用互斥锁。

5.6.6 蜂鸣器驱动源码分析1
	1）dev_init
		1、 信号量
		2、 miscdevice
		3、 gpio_request
		4、 printk

	2）ioctl
		1、 为什么需要ioctl（input output control，输入输出控制）。

		2、 ioctl怎么用

5.6.6 蜂鸣器驱动源码分析2



5.7.1 framebuffer介绍
	1）什么事framebuffer
		1、 裸机中如何操作LCD
		2、 OS下操作LCD有什么难点
		3、 framebuffer帧缓冲（简称fb）是linux内核中虚拟出的一个设备
		4、 framebuffer向应用提供一个统一标准接口的显示设备
		5、 从驱动来看，fb是一个典型的字符设备，而且创建了一个类 /sys/class/graphics

	2）framebuffer的使用
		1、 设备文件 /dev/fb0
		2、 获取设备信息 #include <linux/fb.h>
		3、 mmap做映射
		4、 填充framebuffer

5.7.2 framebuffer应用编程1
	1）打开设备
	2）获取设备信息
		1、 不可变设备而信息 FSCREENINFO，使用ioctl的 FBIOGET_FSCREENINFO名
		2、 可变信息 VSCREENINFO，使用ioctl的 FBIOGET_VSCREENINFO名

5.7.2 framebuffer应用编程2
	1）mmap做映射
		1、 做完了mmap后fb在当前进程中就已经就绪了，随时可以去读写LCD显示器了。

	2）fb显示之刷背景


5.7.2 framebuffer驱动框架总览	
	1）驱动框架部分
		1、 drivers/video/fbmem.c
			1、 创建graphics类、注册fb的字符设备驱动、提供register_framebuffer接口给具体framebuffer驱动编写来注册fb设备的。本文件相对于fb来说，地位和作用和misc.c文件相对杂散类设备来说是一样的，结构和分析方法也是类似的。

		2、 drivers/video/fbsys.c
			1、 这个文件是处理fb在/sys目录下的一些属性文件的。

		3、 drivers/video/modedb.c
			1、 这个文件是管理显示模式的（譬如VGA、720P等就是显示模式）。

		4、 drivers/video/fb_notify.c
			1、 这个用来是来管fb通知的。

	2）驱动部分
		1、 drivers/video/samsung/s3cfb.c
			1、 驱动的主体。

		2、 drivers/video/samsung/s3cfb_fimd6x.c
			1、 很多LCD硬件操作的函数。

		2、 arch/arm/mach-s5pv210/mach-x210.c
			1、 负责提供 platform_device。

		3、 arch/arm/plat-s5pv210/devs.c
			1、 为platform_device提供一些硬件描述信息的。

	3）如何分析
		1、 经验
		2、 分析menuconfig、Makefile、Kconfig等。
		3、 内核编译后检查结果中的.o文件

5.7.06 framebuffer驱动框架分析1
	1）fbmem_init函数
		1、 #ifdef MODULE
		2、 fb_proc_fops和fb在proc文件系统中的表现
		3、 register_chrdev注册fb设备
		4、 class_create创建graphics类
		5、 fbmem_exit的对应

	2）fb_fops
		1、 read/write/mmap/ioctl
		2、 register_fb和num_registerd_fb
		3、 struct fb_info

struct fb_info *registered_fb[FB_MAX] __read_mostly;		数组指针，用来存储fb设备
int num_registered_fb __read_mostly;						当前存在的fb设备个数

5.7.07 framebuffer驱动框架分析2
	1）register_framebuffer
		1、 fb驱动框架开放给驱动编写者的注册接口
		2、 fb_check_foreignness					检查大小端
		3、 remove_confliction_framebuffers		清楚冲突的framebuffer
		4、 device_create 						设备创建
		5、 fb_init_device						
			dev_set_drvdata						暂存fb_info这个结构体（挂接）
			dev_get_drvdata						使用fb_info结构体，主要是因为 show、store方法时解析出fb_info结构体。

	2）fb在sysfs中的接口
		1、 device_fttrs
		2、 dev_set_drvdata和dev_get_drvdata

5.7.08 framebuffer驱动框架分析3
	1）fb的mode
		1、 什么是mode
		2、 fb_var_to_videomode
		3、 fb_add_videomode

	2）注册登记fb设备
		1、 register_fb[i] = fb_info;
		2、 结合fb_read等函数中对fb_init的使用
		3、 关键点：数据如何封装、数据由谁消费、输入如何传递

	3）fb_notifier_call_chain


5.7.09 framebuffer驱动分析1
	1）s3cfb.c
		1、 注意目录结构的组织
		2、 s3cfb_driver

	2）s3c_device_fb
		1、 mach-x210.c中 被使用
		2、 devs.c中
		3、 resource的定义和作用

5.7.10 framebuffer驱动分析2 
	1）probe函数
		1、 struct s3c_platform_fb		这个结构体是fb的platform_data结构体，这个结构体变量就是platform设备的私有数据，这个数据在platform_device.device.platform_data中存储。在mach文件中去准备并填充这些数据，在probe函数中通过传参的platform_device指针取出来。

		2、 struct s3cfb_global		这个结构体主要作用是在驱动部分的2个文件（s3cfb.c、s3cfb_fimd6x.c）的函数中做数据传递用的。
		3、 struct resource 
		4、 regulator

	2）platform_data的传递过程
		1、 to_fb_plat
		2、 s3cfb_set_platdata
		3、 smdkc110_machine_init

5.7.11 framebuffer驱动分析3 
	1）struct s3cfb_gpio
	2）pdata->cfg_gpio
	3）pdata->clk_gpio
	4）resource的处理
		1、 platform_device中提供resource结构体数组
		2、 probe中platform_get_resource取出resource并且按FLAG分头处理。

5.7.12 framebuffer驱动分析4
	1）一些硬件操作
		1、 s3cfb_set_vsync_interrupt	
		2、 s3cdb_set_global_interrupt

	2）s3cfb_init_global
	3）相框架注册fb设备
		1、 s3cfb_alloc_framebuffer
		2、 s3cfb_register_framebuffer

5.7.13 framebuffer驱动分析5
	1）一些硬件操作
		1、 s3cfb_set_clock
		2、 s3cfb_set_window
		3、 s3cfb_display_on

	2）驱动中处理中断
		1、 platform_get_irq
		2、 request_irq

	3）logo显示
		s3cfb_probe
			fb_prepare_logo					真正查找logo
				fb_find_logo				
			fb_show_logo
				fb_show_logo_line
					fb_do_show_logo			真正显示logo
						info->fbops->fb_imageblit		实际操作硬件fb，显示logo的函数

5.7.14 应用层为何不能设置分辨率
	1）问题描述
		1、 在应用层试图修改分辨率失败，原因何在？
		2、 问题定位：肯定是驱动的事儿
		3、 进一步驱动中定位：ioctl部分的事儿

	2）fb的ioctl部分
		1、 fb是典型的字符设备驱动
		2、 ioctl分为2部分，在驱动框架部分和驱动底层部分各有一半
		3、 一路最终找问题
fbmem.c
	fbmem_init
		register_chrdev
			fb_ioctl
				do_fb_ioctl
					fb_set_var
						info->fbops->fb_check_var	（s3cfb_check_var）

	3）让自己的开发板正常显示
		1、 移植的核心是lcd参数的更改


5.8.1 input子系统基础之按键
	1）什么事input子系统
		1、 何为输入设备
		2、 linux中输入设备的编程模型
			1、 命令行界面的输入类设备应用接口
			2、 GUI界面带来的麻烦、不同类的输入设备也会带来麻烦
			3、 struct input_event

		3、 input子系统介绍
			1、 linux的input子系统解决了什么问题：应用层与驱动层标准输入类事件的传递
			2、 input子系统分为4个部分：应用层 + input event + input core + 硬件驱动
			3、 input子系统如何工作
			4、 事件驱动型GUI框架，如QT、VC等。

5.8.2 input设备应用层编程实践
	1）确定设备文件名
		1、 应用层操作驱动有2条路：/dev目录下的设备文件，/sys目录下的一些属性文件的。
		2、 input子系统用的/dev目录下的设备文件，具体一般都是在 /dev/input/xxx


5.8.3 input子系统框架总览
	1）input子系统分为三层
		1、 最上层：输入事件驱动层， evdev.c和mousedev.c
		2、 中间层：输入核心层，	input.c
		3、 最下层：输入设备驱动层，	drivers/input/xxx 文件夹下

	2）input类设备驱动开发方法
		1、 输入事件驱动层和输入核心层不需要动，只需要编写设备驱动层。
		2、 设备驱动层编写的接口和调用模式已经定义好，驱动工程师的核心工作量是对具体输入设备硬件的操作和性能调优。
		3、 input子系统不算复杂，学习时要注意“标准模式”四个字。

5.8.6 input子系统框架核心层分析1
	1）核心模块注册input_init
		1、 class_register
		2、 input_proc_init
		3、 register_chrdev

	2）设备驱动层的接口函数
		1、 input_allocate_device，	kzmalloc，初步初始化（非专业的）
		2、 input_set_capability，	
			input_set_capability(dev, EV_KEY, BTN_LEFT);	// 设置鼠标左键
			input_set_capability(dev, EV_KEY, BTN_RIGHT);	// 设置说表右键
			input_set_capability(dev, EV_REL, REL_X);		// 设置说表左移
			input_set_capability(dev, EV_REL, REL_Y);		// 设置说表右移

		3、 input_register_device


5.8.7 input子系统框架核心层分析2
	1）handler和device的匹配
		1、 input_attach_handler
				input_match_device			匹配device和handler
				handler->connect			链接device和handler

	2）事件驱动层的接口函数
		1、 input_register_handler			类似 input_register_device
		2、 input_register_handle


5.8.8 输入事件驱动层源码分析
	1）



input输入子系统的device_register 在 handler_connect 函数中飞凯注册的。
device_register == 
	device_initialize + device_add

异步通知 阻塞式访问 多路io复用  ？？？

5.8.9 输入设备驱动层源码分析1
	1）先找到bsp中按键驱动源码 （在match文件中）
		1、 锁定目标：半载按键驱动
		2、 确认厂家提供的BSP是否已经有驱动
		3、 找到bsp中的驱动源码

	2）按键驱动源码初步分析
		1、 模块装载源码初步分析
		2、 平台总线相关分析
		3、 确定重点：probe函数

	3）源码细节实现分析
		1、 gpio_request
		2、 input_allocate_device
		3、 input_register_device
		4、 timer
			例如：
				init_timer(&timer);		// 初始化timer
				timer.function = s3cbutton_timer_handler;	// 绑定定时处理函数

				timer.expires = jiffies + (HZ/100);		// 定时时间
				add_timer(&timer);



5.9.1 触摸屏驱动概览
	1）触摸屏驱动概览
		1、 常用的2中触摸屏
			电阻触摸屏。驱动一般分为2中：一种是SoC内置触摸屏控制器，一种是外置专门触摸屏控制芯片，通过I2C接口和SoC通信。
			电容触摸屏。驱动只有一种，外接专用的电容式触摸屏控制芯片，I2C接口和SoC通信。

	2）X210使用的触摸屏
		1、 X210V3使用的触摸屏：ft5x06
		2、 X210V3S使用的触摸屏：gslX680

	3）学习触摸屏驱动的关键点
		1、 input子系统相关知识
		2、 中断上下半部
		3、 I2C子系统
		4、 触摸屏芯片本身知识

	4）竞争状态和同步


5.9.2 内核中的竞争状态和互斥
	1）一些概念
		1、 竞争状态（简称竞态）
		2、 临界段、互斥锁、死锁
		3、 同步（多CPU、多任务、中断）

	2）解决竞态的方法
		1、 原子操作
		2、 信号量、互斥锁	 	可休眠（满足才运行）
		3、 自旋锁 			空耗cpu就是不休眠

	3）自旋锁和信号量的使用要点
		1、 自旋锁不能递归
		2、 自旋锁可以用在中断上下文（信号量不可以，因为可能睡眠交出CPU），但是在中断上下文中获取自旋锁之前要先禁用本地中断。
		3、 自旋锁的核心要求是：拥有自旋锁的代码必须不能睡眠，要一直持有CPU知道释放自旋锁
		4、 自旋锁的核心要求就是：拥有自旋锁的代码必须不能睡眠，要一直持有CPU知道释放自旋锁
		5、 信号量和读写信号量适合于保持时间较长的情况，他们会导致调用者睡眠，因此只能在进程上下文使用，而自旋锁适合于保持非常短的情况，他可以在任何上下文使用。如果保护的共享资源只在进程上下文访问，使用信号量保护该共享内存资源非常合适，如果对贡献内存资源的访问时间非常短，自旋锁也可以。但是如果被保护的共享资源要在中断上下文访问（包括前半部即中断处理句柄和后半部即软中断），就必须使用自旋锁。自旋锁保持期间是抢占失效的，而信号量和读写信号量保持器件是可以被抢占的。自旋锁只有在内核壳抢占或SMP（多处理器）的情况下才真正需要，在单CPU且不可抢占的内核下，自旋锁的所有操作都是空操作。

注意：中断上下文不参与进程调度

5.9.4 中断的上下半部1
	1）中断处理的注意点
		1、 中断上下文，不能喝用户控件数据交互
		2、 不能交出CPU（不能休眠、不能schedule）
		3、 ISR运行时间尽可能短，越长则系统响应特性越差

	2）中断下半部2中解决方案
		1、 为什么要分上下半部（top half，又叫顶半部）和下半部（bottom half，又叫底半部）
		2、 下半部处理策略1：tasklet（小任务）
		3、 下半部处理策略2：workqueue（工作队列）

	3）tasklet使用实战  （软中断，上半部不会被打断，下半部可能会被同类型的中断打断）
		1、 tasklet相关接口介绍
		2、 实战演示tasklet实现下半部
		
5.9.5 中断的上下半部2
	1）workqueue实战演示	（下半部不处于中断上下文，因此可以被cpu调度）
		1、 workqueue的突出特点是下半部分会交给worker thead，因此下半部处于进程上下文，可以被调度，因此可以睡眠。
		2、 include/linux/workqueue.h

	2）实践模式化
		第一步：
			DECLARE_TASKLET(my_tasklet, tasklet_func, 0);
			DECLARE_WORK(my_workqueue, work_queue_func);
		第二部：
			tasklet_schedule(&my_tasklet);
			schedule_work(&my_workqueue);
		第三部：
			进入下半部，执行实际操作。

	3）中断上下半部处理原则
		1、 必须立即进行紧急处理的极少量任务放入在中断的顶半部，此时屏蔽了与自己同类型的中断，由于任务量少，所以可以迅速不受打扰的处理完紧急任务。
		2、 需要较少实践的中断数量的急迫任务放在tasklet中。此时不会屏蔽任何中断（包括与自己的顶半部同类型的中断），所以不影响顶半部对紧急事务的处理；同时又不会进行用户进程调度，从而保证了自己急迫任务得以迅速完成。
		3、 需要较多时间且不急迫（允许被操作系统剥夺运行权）的大量任务放在woekqueue中。此时操作系统会尽量快速处理完这个任务，但如果任务量太大，期间操作系统也会调度别的用户进程运行，从而保证不会因为这个任务需要运行时间将其它用户进程无法进行。
		4、 可能引起睡眠的任务放在workqueue中。因此在workqueu中睡眠是安全的。在需要获得大量的内存时、在需要获取信号量时，在需要执行阻塞式I/O操作时，用workqueue很合适。


5.9.6 linux内核的I2C子系统详解1
	1）I2C总线汇总概览
		1、 三根通信线：SCL、SDA、GND
		2、 同步、电平、低速、串行、近距离
		3、 总线式结构，支持多个设备挂接在同一个总线上
		4、 主从式结构，通信双方必须一个为主（master）一个为从（slave），主设备掌握每次通信的主动权，从设备按照主设备的节奏被动响应。每个从设备在总线中由唯一的地址（slave address），主设备通过从地址找到自己要通信的从设备（本质是广播）。
		5、 I2C主要用途是主SoC和外围设备之间的通信，最大优势是可以在总线上拓展多个外围设备的支持。常见的各种物联网传感器芯片（如gsensor、温度、湿度、光照强度、酸碱度、烟雾浓度、压力等）均使用I2C接口和主SoC进行连接。
		6、 电容触摸屏芯片的多个引脚构成2个接口。一个I2C的，负责和主SoC连接（本身作为从设备），主SoC通过改接口初始化及控制电容触摸屏芯片、芯片通过改接口向SoC汇报触摸事件的信息（触摸坐标等），我们使用电容触摸屏时重点关注的是这个接口；另一个接口是电容触摸板的管理接口，电容触摸屏芯片通过改接口来控制触摸板硬件。改接口是电容触摸板的管理接口，电容触摸芯片通过该接口控制触摸板硬件。该接口是电容触摸屏公司关心的，他们的触摸屏芯片内部固件编程处理这部分，我们使用电容触摸屏的人不关心这里。

	2）linux内核的I2C驱动框架总览
		1、 I2C驱动框架的主要目标是：让驱动开发者可以在内核中方便的添加自己的I2C设备的驱动程序，从而可以更容易的在linux下驱动自己的I2C接口硬件
		2、 源码中I2C相关的驱动位于：drivers/i2c目录下。linux系统提供2种I2C驱动实现方法：第一种叫i2c-dev，对应drivers/i2c/i2c-dev.c，这种方法只是封装了主机（I2C master，一般是SoC中内置的I2C控制器）的I2C基本操作，并且向应用层提供相应的操作接口，应用层代码需要自己去实现对slave的控制和操作，所以这种I2C驱动相当于只是提供给应用层可以访问slave硬件设备的接口，本省并未对硬件做任何操作，应用需要实现对硬件的操作，因此写应用的人必须对硬件非常了解，其实相当于传统的驱动中干的活丢给应用去做了，所以这种I2C驱动又叫“应用层驱动”，这种方式并不是主流，他的优势是把差异化都放在应用中，这样在设备比较难缠（尤其是slave是非标准I2C时）时不用动驱动，而只需要修改应用皆可以实现对各种设备的驱动。这种驱动在驱动层很简单（就是i2c-dev.c）此处暂不分析。
		3、 第二种I2C驱动是所有的代码都放在驱动层实现，直接向应用层提供最终结果。应用层甚至不需要知道这里面有I2C存在，譬如电容式触摸屏驱动，直接向应用层提供 /dev/input/event1 的操作接口，应用层编程的人根本不知道event1中涉及到了I2C。这种后续重点分析。

5.9.7 linux内核的I2C子系统详解2
	1）I2C子系统的4个关键结构体
		1、 struct i2c_adapter		描述主机i2c控制器（适配器）
		2、 struct i2c_algorithm		i2c算法（描述主机i2c跟从机的算法，时序）
		3、 struct i2c_client		i2c（从机）设备信息
		4、 struct i2c_driver		i2c（从机）设备驱动

	2）关键文件
		1、 i2c-core.c
		2、 busses目录				不同的i2c控制器
		3、 algos目录					通信时序

5.9.7 linux内核的I2C子系统详解3
	1）i2c-core.c初步分析
		1、 smbus代码略过
		2、 模块加载和卸载。	bus_register(&i2c_bus_type)

		总结：i2c总线上挂着2条分支：i2c_client链和i2c_driver链，当任何一个driver或者client去注册时，i2c总线都会去调用mathc函数去对client.name和driver.id_table.name进行循环匹配。如果driver.id_table中所有的id都匹配不上则说明client并没有找到一个对应的driver，没了；如果匹配上了则表明client和driver是适用的，namei2c总线会调用自身的probe函数，自身的probe函数优惠调用driver中提供的probe函数，driver中的probe函数会对设备进行硬件初始化和后续工作。

	2）I2C总线的匹配机制
		1、 match函数
		2、 probe函数

	3）核心层开放给其他部分的注册接口
		1、 i2c_add_adapter/i2c_add_numbered_adapter		注册i2c adapter，主机控制器
		2、 i2c_add_driver								注册driver
		3、 i2c_new_devices								注册client

5.9.10 linux内核的I2C子系统详解5
	1）adapter模块的注册
		1、 平台总线方式注册
		2、 找到driver和device，并且确认其配对过程
		3、 probe函数

	2）probe函数分析
		1、 填充一个i2c_adapter结构体，并且调用接口去注册
		2、 从platform_device接收硬件信息，做必要的处理（request_mem_region & ioremap、request_irq等）
		3、 对硬件做初始化（直接操作210内部I2C控制器的寄存器）

	3）i2c_algorithm
		1、 i2c->adap.algo = &s3c24xx_i2c_algorithm
		2、 functionality
		3、 s3c24xx_i2c_doxfer

5.9.11 linux内核i2c子系统详解6
	1）i2c_driver的注册
		1、 以gslx680的驱动为例
		2、 i2c_driver的基本分析：name和probe

	2）i2c_client从哪里来
		1、 直接来源：i2c_register_board_info
			smdkc110_machine_init
				i2c_register_board_info


struct i2c_board_info {
	char		type[I2C_NAME_SIZE];		// 设备名
	unsigned short	flags;					// 属性
	unsigned short	addr;					// 设备从地址
	void		*platform_data;				// 设备私有数据
	struct dev_archdata	*archdata;
#ifdef CONFIG_OF
	struct device_node *of_node;
#endif
	int		irq;							// 设备使用的IRQ号，对应CPU的EINTx
};
		2、 实现原理分析
			内核维护一个链表：__i2c_board_list，这个链表上连接的是i2c总线上挂接的所有硬件设备信息结构体。也就是说这个链表维护的是一个 struct i2c_board_info结构体链表。
			真正需要的是 struct i2c_client在别的地方由 __i2c_board_list链表中的各个节点内容来另外构成。


	函数调用层次：
		i2c_add_adapter
		i2c_add_numbered_adapter
			i2c_register_adapter
				i2c_scan_static_board_info
					i2c_new_device
						device_register

	总结：i2c总线的i2c_client的提供是内核通过i2c_add_adapter/i2c_add_numbered_adapter接口调用时自动生成的，生成的原料是mach-x210.c中的 i2c_register_board_info(1, i2c_devs1, ARRAY_SIZE(i2c_devs1)); 


5.9.14 gslX680驱动源码分析1
	1）



5.10.1 正确理解块设备驱动的概念
	1）块设备和字符设备的差异
		1、 块和字符是两种不同的访问设备的策略
		2、 同一个设备可以同时支持快和字符两种访问策略
		3、 设备本身的物理特性决定了哪一种访问策略更合适
		4、 块设备本身驱动层支持缓冲区，而字符设备驱动层没有缓冲区
		5、 块设备驱动最合适存储设备

	2）块设备驱动特点
		1、 字符设备只能顺序访问（如串口发送数据顺序），而块设备可以随机访问（不连续访问）
		2、 传统的机械式块设备（如硬盘、DVD）虽然可以随机访问，但是连续访问效率更高，因此块设备驱动中有排序逻辑将用户的随机访问重新调整成尽量连续访问以提升效率。
		3、 Nand、SD卡等随机访问效率等同于顺序访问

	3）块设备相关的几个单位
		1、 扇区（Sector），概念来自于早期磁盘，在磁盘、DVD中还有用，在Nand/SD中已经没有意义了，扇区是块设备本身的特性，大小一般为512的整数倍，因为历史原因很多时候都向前兼容定义为512。
		2、 块（block），概念来自于文件系统，室内和对文件系统数据处理的基本单位，大小为若干个扇区，常见有512B、1KB、4KB等。
		3、 段（Section），概念来自于内核，是内核的内存管理中一个页或者部分页，由若干个连续为块组成。
		4、 页（Page），概念来自于内核，是内核内存映射管理的基本单位。linux内核的页式内存映射名称来源于此。

		总结：块设备驱动对下以Sector为单位管理块设备，对上以Block为单位和文件系统交互。
		注意：块设备驱动和字符设备驱动不同，应用层对块设备驱动的访问一般不是直接操作设备文件（/dev/block/xxx，或者/dev/sdax），而是通过文件系统来简介操作。（思考裸机阶段时刷机烧录SD卡时说过的对SD卡的2中访问：文件系统下访问和扇区下访问）

5.10.2 块设备驱动框架简介
	1）块设备驱动框图
		1、 VFS 
		2、 通用块层
		3、 IO调度层（电梯算法）
		4、 块设备驱动层（真正硬件操作部分）

	2）重点结构体
		1、 struct request 				对设备的每一次操作（譬如读或者写一个扇区）
		2、 struct request_queue			request队列
		3、 struct bio 					通用块层bio来管理一个请求
		4、 struct gendisk				表示一个磁盘设备或一个分区


5.10.3 块设备驱动案例分析1
	1）块设备驱动案例演示
		1、 驱动简单介绍
		2、 编译
		3、 模块安装
		4、 查看信息
		5、 挂在测试

	2）块设备驱动简单分析
		1、 如何证明设备驱动真的工作了
			格式化：mkfs.ext2 /dev/my_ramblock
			挂载：mount -t ext2 /dev/my_ramblcok /tmp

		2、 注意各种打印信息
		3、 体会块设备驱动的整体工作框架

5.10.4 块设备驱动案例分析2
	1）源码分析
		1、 register_blkdev（kernel/block/genhd.c）内核提供的注册块设备驱动的注册接口，在块设备驱动框架中的地位，等同于register_chrdev在字符设备驱动框架爱中的地位。
		2、 blk_init_queue 用来实例化产生一个等待队列，将来应用层对本块设备所做的所有的读写操作，都会生成一个request然后被加到这个等待队列中来。
		3、 blk_init_queue(do_my_ramblock_request, &my_ramblock_lock);
			第一个参数：等待队列的回调函数，这个函数是驱动提供的用来处理等待队列中的request的函数（IO调度层通过电梯算法从等待队列中取出一个request，就会调用这个回调函数来处理这个请求）
			第二个参数：是一个自旋锁，这个自旋锁要求驱动提供给等待队列去使用的。

		4、 blk_fetch_request函数是IO调度层提供的接口，作用是从request_queue中（按照电梯算法）取出一个请求（算法认为当前最应该去被执行的一个请求，是被算法排序、合并后的）请求，取出的请求其实就是当前硬件（块设备）最应该去执行的那个读写操作。


5.11.1 网络设备驱动概念
	1）什么是网络设备
		1、 网络设备
		2、 物理网卡：	真正的硬件网卡设备

	2）网络设备接口
		1、 /dev下没有设备文件，也不通过/sys下的属性文件访问。只管看来，应用层都是通过一些特殊的命令（如ifconfig、ping等）来访问网卡硬件（调用驱动）的。本质上应用调用驱动的方法可以通过分析ifconfig、ping等命令的实现来得知。实际就是通过：socket、bind、listen、connect、send、recv等API来实现的。

		2、 网络设备被抽象成一个能发送和接收数据包的“网络接口”
		3、 struct net_device来管理所有网络接口

	3）学习方法
		1、 注意网络设备的访问方法和前两种不同
		2、 2个数据结构（net_device和sk_buff）
		3、 一个虚拟网卡案例代码分析 + DM9000驱动源码分析