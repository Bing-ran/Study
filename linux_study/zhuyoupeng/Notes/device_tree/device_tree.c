10. kernel启动第一阶段分析
	1）
		1、 ldmia	r3!, {r4, r5, r6, r7}	@ 将r3中的值加载到寄存器（多寄存器加载）  ！ == ++
	2、 我们在 __mmap_switched 函数中，将r2的值（本来是是uboot给kernel传参的tag指针值，但是实际上已经是dtb的地址了），赋值给了kernel中的全局变量__atags_pointer，后面到了kernel的第二阶段，相关函数会从这个变量出取这个指针。

	3、 实际上在自解压阶段，已经把uboot传给kernel的tag解开并转成了fdt，并且当时把r2的值赋值为指向dtb的地址指针了。

	4、 第一阶段最后通过 b	start_kernel，跳转到第二阶段。

10. kernel启动第二阶段相关分析
	10.1 start_kernel整体分析
		1、 配合开发板实际启动信息，定位linux_banner
		2、 setup_arch函数是关键，启动时的设备数处理都在这里面
		3、 玩后可以看cpu信息和commandline信息打印
		4、 再往后都是存软件，和2.6内核差没有明显区别。

	10.2 setup_arch函数内部详细分析
		1、 struct machine_desc 非常重要，定义在 \arch\arm\include\asm\mach目录下，作用是用来描述硬件模块的信息的。kernel启动时会解析dtb中的模块信息，然后动态生成machine_desc的结构体变量，然后这些变量会在kernel中使用。

		可以认为 machine_desc 结构体变量，就是dtb的运行时。（将dtb中的信息解到 machine_desc 中）

setup_arch -->
无关的：	setup_processor 	和CPU有关，与设备树无关
	setup_machine_fdt 		解析dtb并且kzmalloc创建 machine_desc 变量实体，然后填充，返回地址值。（参数 __atags_pointer：第一阶段定义的哪一个“汇编数组”）

		early_init_dt_verify 		检验传进来的dtb是不是合法的
		of_flat_dt_match_machine 	检查dtb的根节点的compatible属性是否OK
			arch_get_next_mach

		early_init_dt_scan_nodes	扫描设备树关键节点
			of_scan_flat_dt								检索dtb信息
				early_init_dt_scan_chosen   			检索dtb中的chosen节点（cmdline）
					of_get_flat_dt_prop					传入 prop 返回 value
					early_init_dt_scan_root				检索dtb中的root节点 #size-cells 和 #address-cells
					early_init_dt_scan_memory			检索dtb中的 device_type 节点
				early_init_dt_scan_root					检索root节点
				early_init_dt_scan_memory				检索memory节点
					early_init_dt_add_memory_arch

	machine_desc = mdesc; 全局变量来保证别的地方也可以访问这个 desc 。

	arm_memblock_init(mdesc) --> 

	request_standard_resources --> 

	unflatten_device_tree --> 解包dtb
		__unflatten_device_tree					逐个解压每个节点
			unflatten_dt_nodes					第一次，获取size
			dt_alloc							分配内存
			unflatten_dt_nodes					第二次，解析并填充 of_node
				populate_properties				处理属性
				unflatten_dt_alloc				在这里没有真正alloc，而是 增加了 mem
		of_alias_scan							在这里后处理了alias和chosen节点
	arm_dt_init_cpu_maps --> 提取了dtb中CPU有关的节点，然后操作硬件设置CPU。
		of_find_node_by_path("/cpus");			在 of_root 指向的device_node结构体树种找到cpu节点
	psci_dt_init

无关的：	setup_machine_tags	--> 如果dtb方式失败，尝试老式的tag方式来解析病填充machine_desc。


15. kernel启动第二阶段中dtb的解开
	1）dtb解开之前
		1、 dtb被解开前，对于dtb的检索都是 early 开头的
		2、 dtb解开前，大多是dtb的对根节点的基础信息检索
		3、 dtb解开前，mdesc是关键线索

	2）dtb解开过程
		1、 整个解开过程在 unflatten_device_tree 函数中完成
		2、 调用2次 unflatten_dt_node 函数，通过传参不同实现效果。第一次为计算totalsize，第二次为真的解开每个节点并填充数据结构
		3、 dtb解开后，其实就是很多歌device_node结构体变量形成的梳妆指针链接，头指针就是 of_root 

	3）dtb解开之后
		1、 dtb解开后， of_root 成为了解开后的dtb数据结构的根节点指针，要什么信息就从这里进入检索
		2、 以 cpus 节点为例看看，在 arm_dt_init_cpu_maps 函数中

			/soc/amba/uart0		绝对路径
			serial0				相对路径

最后的总结：
	1、 整个内核启动过程中，设备树有关的部分其实就可以分为4部分：
		第一部分：dtb本身的传递、校验、挪移
		第二部分：early开头的dtb的简单初步的scan
		第三部分：解开dtb生成 of_root 树（次数已经将dtb解析完了，后面不会在访问dtb了）
		第四部分：往后需要某个硬件信息时直接用路径查询 of_root树 找到device_node即可

	2、 本节分析了/cpus这个节点的处理过程案例，这就是典型的
	3、 到这里只是开始而已。大量内容在驱动相关的部分。

18 linux虚拟文件系统中设备树相关分析
	1）proc文件系统
	2）sys文件系统

19 gpio驱动分析
	1）gpio驱动确定
		1、 看dts中gpio节点的compatible属性，在内核中搜索
		2、 在编译过的内核中看drivers/gpio目录，找那些有 .o 的对应的 .c 文件
		3、 查drivers/gpio/Makefile，以及.config中的CONFIG项目对应
		4、 总结：还是要先懂linux的基本驱动框架只是，否则逻辑再强也没法分析

	2）gpio驱动框架组成
		1、 gpio-pl061.c中是HI3516DV300对应的GPIO驱动，准确说是驱动中写的那部分
		2、 gpiolib.c等其他几个是gpiolib，linux gpio框架部分
		3、 gpiolib-of.c是gpiolib对接设备树的部分。

	3）gpio驱动分析
		1、 关键原理都是在核心课程1期的驱动课程中讲过的，并没有太多新的东西。
		2、 关键点1：device_initcall(pl061_gpio_init)
		3、 关键点2：driver和device分离，然后在bus上match与probe
		4、 关键点3：上层实现具体平台封装，框架层实现通用封装

20 GPIO驱动部分源码解析
21 GPIO驱动部分源码解析
	1）

22 驱动代码中的device从哪里来
	drivers/of/platform.c
		arch_initcall_sync(of_platform_default_populate_init);
			of_platform_default_populate(NULL, NULL, NULL);
				of_platform_populate
					of_platform_bus_create						 给root的每一个子节点创建bus
						of_amba_device_create					 这个函数就是我们找的创建device结构体的
							amba_device_add						 真正将device注册到amba总线上的函数

24 uart串口驱动部分简单解析
	1）uart驱动基础分析
		1、 uart驱动确认：drivers/tty/serial/amba-pl011.c
		2、 驱动入口：arch_initcall(pl011_init)
		3、 主体在这：anba_driver_register
		4、 多了一个：platform_driver_register

	2）amba总线match细节
		1、 第一步：找到match函数，确认lookup方法
		2、 第二步：/sys/devices/platform/soc/soc:amba/120a0000.uart/uevent中查看ambaid
		3、 第三步：源码中搜索ambaid的来龙去脉，彻底搞清楚
		4、 学习方法：要么只重使用不重细节，要么按类似方法花时间彻底搞清楚细节

	3）串口驱动分析
		1、 probe理实例化uart_amba_port，并填充，最后pl011_register_port向驱动框架注册
		2、 填充中的uart硬件信息，直接来自于probe参数中的device部分，最终是来自于解析后的设备树
		3、 uart_register_driver和uart_add_one_port所在的serial_core.c就是uart驱动框架的核心
		4、 驱动框架和设备树并无根本关联，实际上解决的是不同问题。