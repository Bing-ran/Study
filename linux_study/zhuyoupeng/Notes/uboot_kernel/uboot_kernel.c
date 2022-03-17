一、Uboot
1、为什么要有uboot
(1) 计算机系统的主要部件
	(1) 计算机系统是由CPU为核心的可以运行的系统。如PC机、嵌入式设备(手机、平板)、单片机(家用电器等)
	(2) 计算机组成部件很多主要包括：CPU + 外部存储器(Flash/硬盘) + 内部存储器(DDR SDRAM/SDRAM/SRAM)
		SOC -- system on chip(包括以上三部分)
(2) PC机的启动过程
	(1) 典型的PC机部署：BIOS程序部署在PC机的主板上(主板出厂时已经预置了)，操作系统部署在硬盘上，
		内存在掉电时无作用、CPU在掉电时不找工作。
	(2) 启动过程：PC上电后先执行BIOS程序(实际上PC的BIOS就在NorFlash上(可直接启动))，BIOS程序
		负责初始化DDR内存，负责初始化硬盘，然后从硬盘上将OS镜像读取到DDR中，然后跳转到DDR中去执行
		OS直到启动(OS启动后BIOS就无用了)

(3) 典型嵌入式系统启动过程
	(1) 嵌入式系统的部署和启动过程都是参考PC机的，只是设备上有一些差别
	(2) 典型的嵌入式系统的部署：uboot程序部署在Flash(能作为启动设备的Flash)上、OS部署在Flash(嵌入式系统中用Flash代替硬盘)上、内存在掉电时无作用、CPU在掉电时不找工作。
	(3) 启动过程：嵌入式系统上电后先执行uboot、然后uboot负责初始化DDR，初始化Flash，然后将OS从Fla    sh中读取到DDR中，然后启动OS(OS启动后uboot就无用了)。

	总结：嵌入式系统和PC机的启动过程几乎没有两样。只是BIOS成了uboot，硬盘成了Flash。

(4) Android系统起启动过程
	(1) android系统的启动和Linux系统(前面讲的典型的嵌入式系统启动)几乎一样。几乎一样：前面完全一样，只是在内核启动后加载跟文件系统后不同了。
	(2) 可以认为启动分为2个阶段：
			第一个阶段：uboot到OS启动；
			第二个阶段：OS启动后到 rootfs (跟文件系统) 加载到命令行执行；Android的启动和Linux的
					   差别在第二阶段。

(5) 总结：uboot到底是干嘛的？
	(1) uboot主要作用是用来启动操作系统内核。

		--> uboot 还要负责部署整个计算机系统。
		--> uboot 中还有操作Flash等板子上硬件的驱动。
		--> uboot 还得提供一个命令行界面工人来操作。

		做以上事情的一个裸机程序。

2、为什么是uboot
(1) uboot 从哪里来的？
	(1) uboot 是sourceForge上的开源项目
	(2) uboot项目的坐着：一个德国人发起的项目。
	(3) uboot 就是由一个人发起，然后由整个网络上所有感兴趣的人共同维护发展而来的一个bootloader。

(2) uboot的发展历程
	(1) 自己使用的小开源项目。
	(2) 被更多人认可使用
	(3) 被soc厂商默认支持

	总结：uboot经过多年发展，已经成为事实上的业内BootLoader标准。现在打不分的嵌入式设备都会默认使用uboot来作为BootLoader。

(3) uboot的版本号问题
	(1) 早期的uboot的版本号类似于这样：uboot1.3.4。 后来版本号变成了类似于uboot-2010.06。
	(2) 两种版本号的区别：uboot的核心部分几乎没怎么变化，越新的版本支持的开发板越多而已，对于一个 老版本的芯片来说，新旧版本的uboot并没有差异。

		末尾 -rc 非正式版、临时版本。
(4) uboot的可以执行的正确理解	。
	(1) uboot就是universal BootLoader(通用的启动代码)，通用的意思就是在各种地方可以用。所以说
		uboot具有可移植性。
	(2) uboot 具有可移植性并不是说uboot在那个开发板都可以随便用，而是说uboot具有在源代码级别的移 植能力，可以针对多个开发板进行移植，移植后就可以咋这个开发板上使用了。

(5) 总结：时势造英雄，任何牛逼的东西都是时代的产物。uboot的出现是一种必然，如果没有uboot也会有另一个BootLoader来代替。

3、uboot常用命令
uboot 的主要目标是启动内核，因此uboot需要部署内核，所以就需要从本地主机下载镜像烧录到flash中。下载方式主要有：fashboot、tftp。

fastboot的方式是通过USB线进行数据传输。
tftp的方式是通过有线网络。典型的方式就是通过网络，fastboot是近些年才新发展的。

(1)tftp方式下载指令
	1） tftp方式下载时实际上uboot扮演的是tftp客户端程序角色，主机windowds或虚拟机ubuntu中必须有 一个tftp服务器，然后将要下载的景象文件放在服务器的下载目录中，然后开发板中使用uboot的tftp命令去下载即可。
	2）有些人习惯在windows总搭建tftp服务器，一般是用一些软件来搭建(譬如tftp32，百度)；也可以在Linux下搭建tftp服务器。
	3）老朱教程tftp命令格式：
		1、检查uboot的环境变量：serverip(要与tftp服务器的ip地址一致，不一定与主机ip一样)
		2、tftp 0x30000000 zImage-qt（将zImage 下载到0x30000000处）
		3、镜像下载到DDR中后，uboot就可以用movi指令进行镜像烧写了。

(2) nfs启动内核命令
	1）uboot中也支持nfs命令。

(3) SD卡/iNand操作指令movi
	1）开发板如果使用SD卡/EMMC/iNand等作为Flash，则在uboot中操作flash的指令为movi(或mmc)
	2）movi指令是一个指令集，有很多子命令，具体可以使用help movi命令查看。
	3）movi的指令都是movi read和movi write一组的，movi read 用来读取iNand到DDR上，movi write用来将DDR中的内容写入iNand中。理解这些治理一定要注意涉及到的2个硬件：iNand和DDR内存。
		例：movi read {u-boot | kernel} {addr}

		movi read ： 使用这个指令不能省略这部分
		{}		  ： 表示必选
		{ | }	  ： 表示 | 前后二选一
		[]		  ： 表示可选参数

		movi read u-boot 0x30000000  表示将iNand中的u-boot读出到0x30000000 (uboot代码中将iNand分成很多个分区，每个分区有地址范围和分区名，uboot程序操作中可以使用直接地址来操作iNand分区，也可以使用分区名来操作分区。)；注意这里的0x30000000 也可以直接写作30000000，意思是一样的(uboot的命令行中所有数字默认当做十六进制处理。)

(4) NandFlash操作之林nand
	1）理解方法类似于movi

(5) 内存操作指令：mm、mw、md
	1）DDR中是没有分区的（只听说过硬盘、Flash进行分区，没听说过堆内存进行分区...），但是内存使用时要注意，千万不能越界猜到别人了。因为uboot是一个裸机程序，不像操作系统会由系统整体管理所有内存，系统负责分配和管理，系统会保证内存不会随便越界。然后裸机程序中uboot并不管理所有内存，内存是散的随便用，所以如果程序员（使用uboot的人）自己不注意就可能出现自己把自己的数据覆盖了。
	（所以你思考为什么把uboot放在23E00000地址处）

	2）md指令(memory display)，用于显示内存中的内容。
	3）mw指令(memory write)，将内容写到内存中。
	4）mm指令(memory modify)，修改内存中的某一块，就是写内存(优势：可以看见修改前的内容，地址连续递增，按y结束)

(6) 启动内核指令：bootm、go
	1）uboot的终极目标是启动内核，启动内核在uboot中表现为一个指令，uboot命令行中调用这个志林就会   启动内核。
	2）区别：
		bootm：启动内核同时给内核传参
		go	 ：命令启动内核不传参

		bootm才是正宗的启动内核命令，一般情况系都用这个；go命令本来不是专为启动内核设计的，go命令内部其实就是一个函数指针指向一个内存地址然后直接调用哪个函数，go命令的实质就是PC直接跳转到一个内存地址去运行而已。go命令可以用来在uboot中执行任何的逻辑程序(有一种调试逻辑程序的方法就是实现启动uboot，然后在uboot中去下载逻辑程序，用go命令去执行裸机程序)

4、uboot的常用环境变量
(1) 如何理解环境变量
	1）环境变量相当于系统的全局变量，可保存(Flash中有一份原始备份，DDR中有一份是运行前拷贝的)

(2) 环境变量如何参与程序运行
	1）环境变量有两份：一份在Flash中，另一份在DDR中。uboot开机时一次性从Flash中读取完全部的环境变量到DDR中作为环境变量的初始化值，然后使用过程中都使用DDR中这一份，用户可以使用saveenv指令将DDR中的环境变量重新写入Flash中去更新Flash中的环境变。下次开机时又会从Flash总再读一次。

	2）环境变量在uboot中使用字符串表示的，也就是说uboot是按照字符匹配的方式来区分各个环境变量的。   因此用的时候一定要注意不要打错字了。

(3) 自动运行倒数时间：bootdelay
	1）
(4) 网络设置：ipaddr serverip
	1）ipaddr是开发板的本地IP地址
	2）serverip是开发板通过tftp指令去tftp服务器下载东西时，tftp服务器的IP地址。
	3）gatewayip是开发板的本地网关地址
	4）netmask是子网掩码
	5）ethaddr是开发板的本地网卡的MAC地址。

(5) 自动运行命令设置：bootcmd
	1）uboot启动胡会开机自动倒数bootdelay秒，如果没人按下回车打断启动，则uboot会自动执行启动命令   来启动内核。
	2）uboot开机自动启动时实际就是在内部执行了bootcmd这个环境变量的值所对应的命令集。

		bootcmd=movi read kernel 30008000; bootm 30008000
		表示：将iNand的kernel分区读取到DDR内存的0x30008000地址处，然后使用bootm启动命令从内存0x30008000处启动内核。

	设置bootcmd：set bootcmd 'movi read kernel 30008000; bootm 30008000'		注意添加 ''

(6) uboot给kernel传参：bootargs
	1）Linux内核启动时可以接受uboot给他传递的启动参数，这些启动参数是uboot和内核约定好的形式、内    容，Linux内核在这些启动参数的指导下完成启动过程。这样的设计是为了灵活，为了内核在补充新编译的情况下可以用不同的方式启动。
	2）我们要做的事情就是：在uboot的环境变量中设置bootargs，然后bootm命令启动内核时会自动将bootargs传递给内核。

		bootargs=console=ttySAC2,115200 root=/dev/mmcblk0p2 rw init=/linuxrc rootfstype=ext3
		意义：console=ttySAC2,115200	  控制台使用串口2，波特率115200
			 root=/dev/mmcblk0p2 rw   跟文件系统使用SD卡端口0设备（iNand）第2分区,跟文件系统
			 						  是可读可写的
			 init=/linuxrc			  Linux的进程1(init进程)的路径
			 rootfstype=ext3		  根文件系统的类型是ext3

	3）内核传参非常重要。在内核移植的时候，新手经常因为忘记给内核传参，或者给内核传递的参数不对，    造成内核启动不起来。

	4）新建、更改、删除一个环境变量的方法
		1、新建一个环境变量，使用 set var value
		2、更改一个环境变量，使用 set var value
		3、删除一个环境变量，使用 set var

	5）注意：环境变量更改后的保存
		1、修改环境变量后一定要保存，否则下次开机更改就又没了。

(7) uboot中对Flash和DDR的管理
	1）uboot阶段Flash的分区
		1、所谓分区，就是说对Flash进行分块管理。
		2、PC机等产品中，因为搭建都是在操作系统下使用硬盘，整个硬盘由操作系统统一管理，操作系统会   使用文件系统帮我们管理硬盘空间。(管理保证了文件之间刽相互堆叠)，于是使用者不用自己太过在
		  意分区问题。
		3）在uboot中是没有操作系统的，因此我们对Flash(相当于硬盘)的管理必须事先使用分区界定
		   (实际上在uboot中和kernel中队有个分区表，分区表就是我们在做系统一直时对Flash的整理管理分配方法)。有了这个界定后，我们在部署系统是按照分区界定方法来部署，uboot和kernel的软件中也是按照这个分区界定来工作，就不会错了。

		 4）分区方法不是一定的，不是固定的，是可以变动的。但是在一个一直中必须事先设计好定死，一般   在设计系统一直时就会定好，定的标准是：uboot：uboot必须从Flash起始地址开始存放（也许   是扇区0，也许是扇区1，也许是其他，取决于SOC的启动设计），uboot分区的大小必须保证uboo   t可能能放下，一般设计为512KB或者1MB（因为一般uboot坑定不足512KB，给再大其实也可以工   作，但是浪费）；

		 环境变量：环境变量分区一般紧贴着uboot来存放，大小为32KB或者更多一点。
		 kernel：kernel可以紧贴环境变量存放，大小一般为3MB或者5MB或其他。
		 rootfs：......
		 剩下的就是自由分区，一般kernel启动后将自由分区挂载到rootfs下使用

		 总结：一般规律如下
		 		1、各分区彼此相连，前面一个分区结尾就是后一个分区开头。
		 		2、整个flash充分利用，从开头到结尾。
		 		3、uboot必须在flash开头，其他分区相对位置是可变的。
		 		4、各分区大小由系统移植工程师自己来定，一般定位合适大小(不能太小，太小容易溢出；
		 		  不能太大，太大浪费空间)
		 		5、在系统移植前确定分区表，在uboot中和kernel中使用同一个分区表。将来在系统部署时和系统代码中的分区方法必须一样。
	2）uboot阶段DDR的分区
		1）DDR的分区和Flash的分区不同，主要是因为Flash是掉电存在的，而DDR是掉电消失，因此可以说D   DR是每次系统运行时才开始部署使用的。
		2）内存的分区主要是在Linux内核启动起来之前，Linux内核启动后内核的内存管理模块会接管整个    内存空间，那时候就不用我们来管。
		3）注意内存分区关键就在于内存中那一块用来干什么必须分配好，以避免各个不同功能使用了同一块   内存造成的相互踩踏。譬如说我们 tftp 0x23E00000 zImage，去下载zImage到0x23E00000处
		   就会出错，因为这个内存实际是uboot的镜像所在。这样下载会导致下载的zImage吧内存中的
		   uboot给冲掉。


5、shell和Makefile
(1) shell介绍
	1）shell是操作系统的终端命令行，是人机交互的一种方式。
	2）我们可以使用shell和操作系统、uboot等软件系统进行交互。具体来说就是我们通过shell给软件系统输入命令然后回车执行，执行完后又回到shell命令行可以再次输入命令执行。
	3）上述的操作方式一般工作很好，但是有缺陷。譬如我们要在Linux下创建a.c，可以touch a.c 但是如果我们现在要在Linux下创建100个文件，分别为a1.c 、 a2.c ...... a100.c 。这时如果手工创建则非常麻烦，这时使用shell脚本区创建则非常简单。
(2) shell是一类编程语言。
	1）编写shell脚本时使用的语言就是shell语言，又叫脚本语言。
	2）shell脚本其实是一类语言而不是一个语言。

(3) 常用的shell语言：sh 、 bash 、 csh 、 ksh 、 perl 、 python等。
	1）在Linux下常用的脚本语言就是bash、sh；
	2）perl、python这样的高级shell脚本语言，常用在网络管理配置等领域，系统运维人员一般要学习这些。
	3）脚本语言一般在嵌入式中应用，主要用来做配置。(一个复杂的嵌入式程序都是可配置的，配置过程就是用脚本语言来实现的)自然不会使用过于复杂的脚本语言特性，因此只需要针对性的学习即可。
	4）Linux下最常用的脚本语言就是bash，我们学习也是与bash为主。

(4) shell脚本的运行机制：解释运行
	1）C语言(C++)这种编写过程是：编写出源代码(源代码是不能直接运行的)然后编译连接形成可执行二进
	   制程序，然后才能运行；而脚本程序不同，脚本程序编写号后源代码即可直接运行(没有编译链接过程)
	2）shell程序是解释运行的，所谓解释运行就是说当我们执行一个shell程序时，shell解析器会逐行的解   释shell程序代码，然后一行一行的去运行。(顺序结构)
	3）CPU实际只认识二进制代码，根本不认识源代码。脚本程序源代码其实也不是二进制代码，CPU也不认识    ，也不能直接执行。只不过脚本程序的编译链接过程不是以脚本程序源代码为单位进行的，而是在脚本
	   运行过程中逐行的解释执行时才去完成脚本程序源代码转成二进制的过程(一定是编译链接，因为这行
	   脚本程序可能早就编译链接好了，这里我们只是调用它)的。

	4）编辑器、编译器、运行方法(脚本的3中执行方法)
		编辑器：shell程序是文本格式的，只要是文本编辑器都可以。但是因为我们的shell是要在Linux
		       系统下运行的，所以换行符必须是'\n'，而windows下的换行符是'\r\n'，因此windows
		       中的编辑器写的shell不能再Linux下运行。
		编译器：不涉及，因为shell是解释性语言，直接编辑完就可以运行。
		运行方法：
				第一种：./xx.sh  		注意权限 chmod a+x xx.sh
				第二种：source xx.sh 	source是Linux的一个命令，这个命令就是用来执行脚本程序					 的。这样不需要脚本具有可执行权限。
				第三种：bash xx.sh  		     
	5）hello world程序和解释
		第一行：一般都是以 #!+ pathname ，例如：  #!/bin/sh  也可以是 #！/bin/bash    
			   解析：指定shell程序执行时被那个解析器解析执行。
		注释：以 # 号开始
		正文：由很多行shell语句构成
	6）shell并不神秘
		1、shell就是以前命令行中键入执行的命令写成了程序
		2、shell编辑完全可以直接运行（不需要编译）



6、Makefile
(1) makefiel基础
	1）Makefile的作用和意义
		项目中c文件太多管理不方便，因此Makefile来做项目管理，方便编译链接过程。例：uboot、kernel

	2） 目标、依赖、命令
		目标：make xxx的那个xxx，就是我们最终要生成的东西。
		依赖：就是生成目标的原材料
		命令：加工方法，所以make xxx的过程就是使用依赖加工成目标的过程。

	3） 统配符%合Makefile自动推导（规则）
		%	   ： Makefile中的通配符，代表一个或几个字母。也就是说 %.o 就代表所有以 .o 结尾的文件。
		自动推导：就是Makefile的规则。 当Makefile需要某一个目标时，他会把这个目标去套规则说明，一但套上某个规	  则说明，则Makefile会师徒寻找这个规则中的依赖，如果能找到则会执行这个规则用依赖生成目标。

	4） Makefile中定义和使用变量
		Makefile定义和使用变量，和shell脚本中非常相似。相似是说：都没有类型，直接定义使用，引用变量时用$var

	5）伪目标（.PHONY）
		伪目标：这个目标本身不代表一个文件，执行这个目标不是为了得到某个文件或东西，而是单纯为了执行目标下面的    	命令。一般都没有依赖。只是为了执行下面的命令，没有依赖：无条件执行。

		使用：	(1) clean:
				(2) .PHONY clean

	6）Makefile的文件名
		Makefile 或者 Makefile
	7）Makefile中引用其他Makefile （include 命令）
		有时候Makefile总体比较复杂，因此分成好几个Makefile来写。然后在主Makefile中引用其他的，用 include 指令来应用。 引用原地展开，和c语言中的 include 类似。

(2) 补充学习
	1）Makefile中的注释：#
		使用：#
	2）命令前面的@用来静默执行
		在Makefile的命令行中前面的 @ 表示静默执行。
		Makefile中默认情况下在执行一行命令前会先把这行命令打印出来，然后在执行这行命令。
		如果不想看到命令本身，只想看到命令执行结果就静默执行。

	3）Makefile中几种变量赋值运算符
		(1) =		最简单的赋值
		(2) :=		一般也是赋值
			以上两个大部分情况下的效果是一样的，但是有时候不一样。
			 =  赋值的变量，在被解析时它的值取决于最后一次赋值时的值，所以你看变量引用的值时不能只往前面看，还要往后   面看。
			：= 赋值的变量，只用看执行语句前面的赋值。解析谁看谁
		(3) ?=		如果变量前面并没有赋值(没有被定义)过则执行这条赋值语句，如果前面已经赋值过了则本行被忽略。
		(4) +=		用来个一个已经赋值的变量接续赋值，意思解释把这次的值加到原来的值后面，有点类似strcat （在shell、Makefile等文件中，可以认为所有的变量都是只读串， += 就相当于字符串strcat接续内容） （注意一个细节， += 续借的内容和原来的内容之间会自动添加一个空格隔开）

	4）Makefile环境变量
		Makefile中用 export 导出的就是环境变量。一般情况如下要求环境变量名用大写，普通变量用小写。
		环境变量：整个工程中所有Makefile之间共享的全局变量。
		普通变量：当前Makefile中使用的局部变量。
		注意：定义了环境变量可能会影响别的Makefile

		Makefile中有一些环境变量时Makefile本身自己定义的内部环境变量 或者 是当前的执行环境提供的环境变量(例如：make CC=arm-linux-gcc)。类似：C语言中编译器预定义的宏 __LINE__ __FUNCTION__等一样。

	5）Makefile中使用通配符
		(1) *		若干个任意字符
		(2) ?		一个任意字符
		(3) []		将[] 中的字符一次拿出来与后面的结合后去匹配
	
		% 也是通配符，表示任意多个字符，和*相似，但是 % 一般只用于规则描述符中，又叫做规则通配符( %.o : %.c)。	

		《跟我一起学Makefile》

	6）Makefile的自动变量
		(1) 为什么使用自动变量？
			在有些情况下文件集合中文件非常多，描述的时候很麻烦，所以我们Makefile就用一些特殊的符号来代替符合某种条件的文件集，这就形成了自动变量。

		(2) 含义：
			预定义的特殊意义的符号。类似于C语言编译器中预置的宏 __LINE__ 一样。

		(3) 常见自动变量：
			$@		规则的目标文件名
			$<		规则的依赖文件名（规则的第一个文件名）
			$^		依赖的文件集合

7、uboot配置编译实践
1）找到官方移植好的uboot（BSP概念）
	(1) 源头的源代码是uboot官网下载的。这个下载的源代码可能没有你当前使用的开发板的移植，甚至找不到当前开发板使用的SO    C对应的移植版本。
	(2) SOC厂商在推出一款SOC后，厂商的工程师会去uboot官网下载一个uboot，根据自己的SOC进行第一步的移植，移植的目标是    厂商 推出的开发板。(譬如三星的S5PV210芯片厂商出的开发板就叫SMDKV210)。所以三星的工程师移植的uboot是根据他们
		自己的SMDKV210开发版移植的。
	(3) 具体的开发板供应商（譬如x210的的开发商深圳市九鼎科技）首先购买三星的SMDKV210开发板，然后进行裁剪（把一些无用	的接口功能裁剪掉，配置降低下，某些配置可能被替换）。硬件替换和裁剪之后生成的新的开发板（譬如x210）和三星官方     的SMDKV210有所不同，因此uboot也不同。但是因为SOC是相同的，所以相似度至少有60%以上。所以具体开发板供应商会以三	星SMDKV210中移植的uboot为蓝本来移植得到自己的开发板的一个uboot移植。我们买x210开发板时厂商光盘中带的BSP中 的uboot源码就是它一直过的。

	总结uboot可以有三种获取途径。 uboot官方、SOC官方、具体开发板的官方。

2）在Linux原生目录下配置编译
	(1) X210移植过的uboot在开发板光盘的BSP中。
	(2) BSP就是 board support package(板级支持包，一般由开发板供应商提供)，里面的内容就是这个开发板相关的源代码、
		文档、教程。
	(3) X210 bsp路径：F:\朱有鹏播放\朱有鹏\210开发板光盘资料\X210V3S_B\linux\QT4.8\bsp

3）配置
	(1) uboot和Linux kernel等复杂项目，都不能直接编译，都要先配置才能编译。
	(2) uboot也要先配置，首先 进入uboot根目录，然后在根目录下执行：make x210_sd_config。
4）编译
	(1) 编译之前一定要注意检查arm-linux-gcc队不对，检查分两步：
		第一步：检查当前编译环境有没有安装合适的arm-linux-gcc。
		第二部：检查当前目录下（uboot根目录）的Makefile中编译器的设置是否正确。在	工程的Makefile中会设置交叉编译工具链的路径和名字，必须与我们装的一直。
		编译：make -j2
			(make 找不到arm-none-linux-genuabi-gcc，可能是64位系统安装32位库
				执行：sudo apt-get install lib32z1 
					or sudo apt-get install lib32ncurses5 )

	du -h u-boot.bin 	--> 	384K	u-boot.bin

5）uboot源码目录分析
	(1) 九鼎官方和三星原版uboot对比
		1、以九鼎为蓝本来学习的，以三星官方的这份为对照。不同版本的uboot或者同一版本
			不同人移植的uboot，可能目录结构和文件内容都有所不同。将来学习了就可以根据需要
			去添加/删除/更改目录结构。
		2、九鼎在以三星的uboot为原材料进行移植时，吧三星版本得到uboot中很多不必要的文件夹、
			文件给删除了

	(2) 各文件介绍

		1、文件
			(1) .gitignore。版本管理工具，与uboot无关。
			(2) arm_config.mk 后缀为.mk ,是一个Makefile文件，将来在某个Makefile中回去调用。
			(3) 三个CHANGELOG文件。修改记录文件，改文件记录了这个uboot项目的版本变迁以及每个版本    变迁较上个版本修改的记录。维护uboot的人员使用。
			(4) config.mk 与 arm_config.mk性质类似。
			(5) COPYING。版权声明，uboot本身是GPL许可证。
			(6) CREDITS。uboot贡献人员记录。
			(7) image_split。一个脚本，看说明是用来分割uboot.bin到BL1的，暂时用不到，先不管。
			(8) MAINTAINERS。维护者。当前社区中维护uboot的人。
			(9) MAKEALL。脚本，应该是帮助编译uboot的脚本。
			(10) Makefile。uboot源代码的注Makefile，将来整个uboot被编译时及时用这个Makefile编 译的。
			(11) mk。快速编译的脚本。 清理 --> 配置 --> 编译。
			(12) mkconfig。是uboot配置阶段的主要配置脚本。uboot的可移植性很大程度就是靠这个配置 脚本维护的。
			(13) mkmovi。脚本。与iNand/SD卡启动有关。
			(14) rules.mk。uboot的Makefile的使用规则，很重要但不去分析。

			总结：以上文件对我们比较重要且必须认真看的有两个：mkconfig和Makefile。一个负责配置，      一个负责编译。

		2、文件件
			(1) api。	硬件无关的功能函数的API。uboot 移植时不用管，这些函数是uboot本身使用的。
			(2) api_examples 。API相关的测试示例代码。
			(3) board。 该文件夹下每一个文件都代表一个开发板，这个文件下面放的文件就是用来描述这 一个开发板的信息的。board目录下有多少个文件夹，就表示当前这个uboot已经被移植到	多少个开发板上了（当前的uboot支持多个开发板）。
			问题一：思考uboot如何支持多套开发板，如何具有可移植性。
			问题二：board下有这么多文件夹，究竟如何确定具体使用的事哪一个？
					uboot在配置阶段会有一些手段帮助我们来确定具体使用的事那个board目录下的哪一个文件夹。（想想为什么不能直接编译而要先配置。）
			问题三：开发板越来越多，board目录下文件夹越来越多不方便管控。于是乎uboot就新增了一种	机制，可以再board目录下不直接方开发板目录，而是在board下放厂家目录（vendor	目录，以具体场上名字命名），然后将这个IC厂商的所有芯片开发板都丢到这个vendor	 目录下去。 多了这层目录会应将配置阶段，在uboot的配置阶段要注意配置时的路径深	 度和存放要对应，不然配置后编译时找不到文件编译就会失败。历史原因造成的兼容新麻	  烦。嘴还是时board目录下就是开发板名字，后来才改成厂商名字的。但是因为要向前兼容	，所以有一部分仍然在board目录下。

			注意：uboot的配置阶段(其实就是根目录下的mkconfig脚本和Makefile中配置有关的部分)主要解决的问题就是在可移植性领域能够帮助我们确定具体的文件夹的路径，然后编译时可以找到应该找到的文件，才能编译成功。因此board目录下的不同会造成配置时的不同。

			(4) common。放的是架构无关，与硬件无关的普遍适用的文件。譬如：控制台实现、crc检验。主 要是两类：cmd开头的，实现uboot阶段的命令；env开头的，是实现环境变量的。
			(5) cpu。 SOC相关的，SOC相关初始化和控制代码（譬如CPU的中断、串口等SOC内部外设的，包	括start.S也是在这里）。

			注意：这个文件夹是严格和硬件相关的，因此移植时也要注意的。但是因为这个文件夹内都是SOC有关的，我们的板子和三星的板子SOC相同，所以几乎不用改动。
			(6) disk。跟磁盘有关。
			(7) doc。文档目录，存放uboot相关的文档。
			(8) drivers。 裸机驱动，从Linux源代码中抠出来的原封不动的Linux设备驱动，主要是开发板	一些必须的驱动，如网卡、Inand/SD卡、NandFlash等的驱动。（uboot是裸机程序，所以	还是有些不同，此处的驱动是Linux驱动的一部分）
			(9) examples。实例代码，没有过。
			(10) fs。 filesystem，文件系统管理Flash等资源。
			(11) include。头文件。
			(12) lib_开头的。典型的 lib_arm和lib_generic，架构相关的库文件。
					lib_arm ： arm相关的
					lib_generic : 所有架构通用的库文件。
			(13) libftb。设备数相关的
			(14) nand_spl。 nand相关的。
			(15) net。nat、tcp、tftp、平命令相关实现。
			(16) onenand开头的。onenand相关的的代码，三星加的。
			(17) post。
			(18) sd_fusing。这里的代码实现了烧录uboot镜像到SD卡的代码。仔细研究。
			(19) tools。工具类代码。譬如：mkimage

			总结：文件夹里面比较重要的，后面会分析涉及到的有：board、common、cpu、drivers、include、lib_arm、sd_fusing、lib_generic。

6）uboot配置和编译详解
(1) Makefile分析。
	1、 uboot version确定
		uboot版本号分三个级别：
			VERSION = 1			主版本号
			PATCHLEVEL = 3		此版本号
			SUBLEVEL = 4		再次版本号
			EXTRAVERSION =		另外附加的版本信息(一般是个人自己写的)

			U_BOOT_VERSION = $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION)

			VERSION_FILE = $(obj)include/version_autogenerated.h
			编译过程中生成，里面就是一个版本宏：#define U_BOOT_VERSION "U-Boot 1.3.4"

	2、 HOSTARCH和HOSTOS
		Makefile执行shell命令。方法一：`pwd`  第二种：$(shell pwd)
		| 管道。将前一个的输出作为后一个的输入。

		HOSTARCH：主机 架构
		HOSTOS：主机 系统
		
	3、 静默编译
		平时默认编译时命令行会打印出来很多编译信息。但是有时候我们不希望看到这些编译信息，
			就后台编译即可。这就叫静默编译。
		使用方法：make -s , -s会传递给Makefile，在50--54行出SECHO就会为空。

	4、2 中编译方法(原地编译和单独输出文件夹编译)
		1、默认情况下，当前文件中的.c文件，编译出来的.o文件会放在同一文件下，这种叫做原地编译
			好处：原地编译处理起来简单。
			坏处：第一污染了源文件目录。第二的缺陷，一套源代码只能按照一种配置和编译方法进行       处理，无法同时维护2个或者2个以上的配置编译方式。

		2、 单独输出文件夹编译(linux kernel也支持)。
			在编译时另外制定一个输出目录，将来所有的编译生成的.o或者生成的其他文件全部丢到那个输出目录下去。源代码目录不做任何无源，这样输出目录就承载了本次配置编译的所有结果。
				
			具体用法：(1) make O=dir 				56----75行
					 (2) export BUILD_DIR=dir  		78 - 123
					 	 make 
					 两种都有的情况下，前者优先级更高。

			OBJTREE ： 编译出的 .o 文件树根目录。默认编一下，为当前目录，在O=  ，为到处的目录。
			SRCTREE ： 源码目录，源码的根目录。  默认编译，以上两个相等，o= 下，不相等。
			TOPDIR  ： 顶层目录

			MKCONFIG： Makefile中的一个变量。它的值就是源码根目录下的 mkconfig(一个脚本)，配置阶段的配置脚本，
					   非常重要。  101行

			include $(obj)include/config.mk 配置过程中由我们的配置生成的 make x210_sd_config 
					值：	
						ARCH   = arm 			架构
						CPU    = s5pc11x		cpu
						BOARD  = x210 			主板
						VENDOR = samsung		厂家
						SOC    = s5pc110		soc

					在下一行(134) export 导出这五个环境变量。
					这里的配置值来自于 2589 行哪里的配置项。

			ARCH ： 会影响后面的 CROSS_COMPILE

			CROSS_COMPILE : 交叉工具链前缀。	后面用时加上 相应的后缀就可以用了。前后缀分开的原因还有：
							不同架构上的交叉编译器只是前缀不一样。 136 -- 182

					实际运用时：make CROSS_COMPILE=xxxx ,编译传参可以覆盖直接在Makefile中的CROSS_COMPILE。

			include $(TOPDIR)/config.mk 主Makefile185行
			编译工具定义(config.mk 94 -- 107)
			包含开发板配置项目(config.mk 112行)
				这个文件作用，知道整个uboot的编译过程。这个内容就是CONFIG_开头的宏 (可以理解为变量)，这些宏/变量会影响我们uboot编译过程的走向，这个条件编译增加可移植性。
			 	这个文件的源材料为 $(SRCTREE)/include/xxx.h头文件。移植uboot主要配置这些头文件。

			 LDSCRIPT : (config.mk 142-149) 链接脚本

			 TEXT_BASE : (config.mk 156 -- 158 ) 
			 		Makefile中在配置x210开发板时，在board/samsung/x210 目录下生成一个
			 		config.mk ，其中的内容就是 TEXT_BASE = 0xc3e00000就相当于一个变量。

			 		作用： 将来整个uboot连接时制定的连接地址。因为uboot中启用了虚拟地址映射，因此c3e00000地址就等于23e00000(根据实际情况)。

			 自动推到规则：

	5、 Makefile 中第一个目标 all
		all 目标 包含很多，具体见源码。
		目标中有一些比较重要。例如：u-boot就是最终编译链接生成的elf格式可执行文件。

7、 配置详解
	(1) make x210_sd_config 
	@$(MKCONFIG) $(@:_config=) arm s5pc11x x210 samsung s5pc110
		$(@ : 表示目标
		: 表示加工
		_config= ：表示用空替代 _config
		最后：x210_sd_config --> x210_sd 

		$1 : x210_sd
		$2 : arm
		$3 : s5pc11x
		$4 : x210
		$5 : samsung
		$6 : s5pc110

		所以: $# = 6

			mkconfig 文件中23 行后结果： BOARD_NAME=x210_sd
			  25行：$# 小于 4 ，exit 1 (mkconfig脚本返回1)
			  26行：$# 大于 6 ，exit 1 (mkconfig脚本返回1)

			  mkconfig传参只能更是 4、5、6

			  33 -- 118 之间都是创建符号链接。为什么创建符号链接？
			  	这些符号链接文件的存在就是整个配置过程的核心，其主要作用是给头文件包含等过程提供指向性链接。根本目的是让uboot具有可移植性。

			  	uboot可移植性的实现原理：在uboot中有很多彼此平行的代码，格子属于格子不同的架构/CPU/开发板，我们在具体到一个开发板的编译时用符号链接的方式提供一个具体的名字的文件夹供编译时使用。这样就可以在配置的过程中通过不同的文件，就可以正确的包含正确的文件。

			 创建的符号链接：
			 	(1) include目录下 asm --> asm-arm   
			 	(2) include目录下 arch --> arch-s5pc110  87 行
			 	(3) include目录下 regs.h --> s5pc110.h
			 	(4) include/asm-arm  proc --> proc-armv

			 	总结：这几个链接在写代码过程中，头文件包含非常有用。例如
			 		 #include <asm/xx.h>
			mkconfig.mk 123--129
				创建include/config.mk文件。
				ARCH=
				CPU=
				BOARD=
				VENDOR=
				SOC=
				目的是让主Makefile在133行包含。

				思考：uboot的配置和编译过程的配合。编译的时候需要ARCH=arm、CPU=xx...等这些变量来指导编译，配置的时候就是为了编译阶段提供这些变量。那为什么不在Makefile中直接定义这些变量去使用，而要在mkconfig脚本中创建config.mk文件然后又在Makefile中include这些文件呢？

				理解这些脚本时，时刻要记住当前所处的路径。

			mkconfig.mk 134 -- 141
				创建(默认情况)/追加(make -a时追加) include/config.h头文件。
				这个文件里面就只有一句： #include <configs/x210_sd.h>，这个文件是我们移植x210开发板时，对开发板的宏定义配置文件。这个文件是我们移植x210时最主要的文件。
				x210 -生成-> autoconfig.mk

			uboot的整个配置过程，很多文件之间是有关联的（有时候这个文件是在哪个文件中创建出来的；有时候这个文件被那个文件包含进去；有时候这个文件是由哪个文件的内容生成的决定的）
			uboot中配置和编译过程，所有的文件或者全局变量都是字符串形式的（壮丽的字符串不是C语言的字符串的概念，指的是都是字符组成的序列）。这意味着我们整个uboot的配置过程都是字符串匹配的，所以要注意大小写、字符正确性，不然很容易出现莫名其妙的错误。

		@echo "TEXT_BASE = 0xc3e00000" > $(obj)board/samsung/x210/config.mk


	(2) uboot的连接脚本
			1、 uboot发连接脚本与裸机中的脚本本质上一样，只是复杂些。
			2、 ENTRY(_start)		用来指定整个程序的入口地址，类似于C语言中main函数的作用。
			3、 SECTIONS				连接脚本的指定。
				指定程序的链接地址有2中方法：一种是在Makefile中ld的flags用-Ttext 0x20000000来
				指定；第二种是在连接脚本的SECTIONS开都用 .=0x20000000来指定。这两种都可以实现相同的效果。其实这两种技巧其实可以配合使用的，也就是说既可以在链接脚本中指定也在ld flags中用-Ttext来指定。但是-Ttext的优先级高。
				uboot的最终连接起始地址就是在Makefile中用-Ttext来指定的，具体见2.4.5.3节，注意TEXT_BASE变量。最终来源是Makefile中配置对应的命令中。

			4、 . = ALIGN(4)  		四字节对其。
			5、 在代码段中注意文件排列顺序。指定必须放在前面部分的那些文件就是那些必须安排在前16kb    内的文件，这些文件中的函数在前16KB会被调用。在后面第二部分(16KB之后)中调用的程序，前后顺序就无所谓了。
			6、 链接脚本中出了 .text .data .rodata .bss段等编译工具自带的段之外，编译工具还允许我们自定义段。例如uboot中 .u_boot_cmd 段就是自定义段。
				自定义段很重要。
8 源码分析
	1、 启动第一阶段 （汇编阶段 内部SRAM运行）
		(1) start.S引入	（由连接脚本 ENTRY(_start) 语句指定 _start 所在处的代码。）
			_start: b	reset
			_start		其实就是一个地址(b	reset 的地址)

		(2) 头文件包含
			#include <config.h>		include目录下，配置过程中生成，内容为：
									#include <configs/x210_sd.h>
									x210_sd.h --> 里面有很多宏

			make x210_sd.h --> ./mkconfig ... --> echo "#include <configs/x210_sd.h>" 
							>> include/config.h

			#include <version.h> include/version.h 包含 #include "version_autogenerated.h"
					Makefile --> include/version_autogenerated.h里面只有一句字符串
					uboot启动时会通过串口打印出uboot版本号，那个版本信息就是来自这里。

			#include <asm/proc/domain.h>  asm目录不是uboot的原生目录，uboot中本来没有这个目录	。asm目录是配置是创建的符号链接，实际指向的就是asm-arm（mkconfig脚本中）

		(3) 启动代码的16字节头部

			.world 伪指令

			1、 裸机中，在SD卡启动/Nand启动等整个镜像开头需要16字节的校验头。(mkv210image.c 中就是为了计算这个校验头)。我们以前做裸机程序时根本没考虑这16字节的校验头，因为：1、如果我们是usb启动直接下载的方式启动的则不需要16字节校验头(irom application note); 2、 如果是SD卡启动mkv210image.c中会给源镜像前加16字节的校验头。
			2、 uboot这里start.S中在开头位置放了16字节的填充占位，这个占位的16字节只是保证正式image的头部确实有16字节，但是这16字节的内容是不对的，还是需要后面去计算校验和然后重新填充的。
			
		(4) 异常向量表构建 一级中断向量表
			1、 异常向量表是硬件决定的，软件知识参照硬件设计来实现的。
			2、 异常向量表中每一种异常都应该被处理，否则真遇到这种异常就跑飞了。但是我们在uboot中并   未 非常细致的处理各种异常。
			3、 复位异常出的代码是： b reset，因此在CPU复位后真正去执行的有效代码是reset处的代码 ，因此reset福好处才是真正有意义的代码开始的地方。
				b	reset						复位异常
				ldr	pc, _undefined_instruction	未定义异常
				ldr	pc, _software_interrupt		软中断异常
				ldr	pc, _prefetch_abort			预期指异常
				ldr	pc, _data_abort				数据异常
				ldr	pc, _not_used			
				ldr	pc, _irq					
				ldr	pc, _fiq

			4、 有意思的 deadbeef 
				意义：.balignl 16,0xdeadbeef 
					  伪指令，让内存以16为单位对齐，如果当前地址不对齐则自动向后走地址知道对其，并向后走的那些内存要用 0xdeadbeef 填充。
					  0xdeadbeef 是16进制的数字，组成这个数字的十六进制全是abcdef之中的字母，而	且这8个字母行号组成了英文的dead beef这个两个单词，字面意思是坏牛肉。

					  为什么要对其？有时候是效率的要求，有时候是硬件的要求。

			5、 TEXT_BASE等

				_TEXT_BASE:				：号前的为标号
					.word	TEXT_BASE   .word 表示定义一个四字节的变量；TEXT_BASE这个变量的值

				这100行这个TEXT_BASE就是Makefile配置阶段时的 TEXT_BASE ,就是连接时指定程序的
				链接地址。
				源代码中和配置Makefile中很多变量是可以互相运送的。简单来说就是有些符号的值可以从Makefile中传递到源代码中。

				CFG_PHY_UBOOT_BASE		33e00000  uboot在我们内存(ddr)中的物理地址。

				
			6、 reset 
				(1)
				msr	cpsr_c, #0xd3		@ I & F disable, Mode: 0x13 - SVC
				意义：向 cpsr寄存器中的c位 写 0xd3 。 cpsr程序状态寄存器
					 将cpu设置为禁止IRQ、FIQ，ARM状态，SVC模式。
					 其实ARM CPU在复位时默认就会进入SVC模式，但是这里还是使用软件将其设置为SVC
					 	模式。整个uboot工作时CPU一直处于SVC模式。
				
				(2) 设置L2、L1cache和MMU
				bl	disable_l2cache 			禁止L2 cache
				bl	set_l2cache_auxctrl_cycle 	L2 cache相关
				bl	enable_l2cache				使能L2 cache
				刷新 L1 cache的icache 和 dcache。
				关闭MMU。(跟虚拟地址相关，此处还没有做虚拟地址映射)

				总结：以上5条都是和cpu的cache和mmu相关的，不用去看，大概知道即可。

				(3) 识别并暂存启动介质选择 
					1、 从哪里启动是由SOC的OM5：OM0这6个引脚的高低电平决定的。
					2、 实际上在210内部有一个寄存器 (地址是0E0000004) ，这个寄存器中的值是硬件根	据OM引脚的设置而自动设置值得。这个值反应的就是OM引脚的接法（电平高低）	，也就是真正的启动介质是谁。
					3、 我们代码中可以通过读取这个寄存器的值然后判断其值来确定当前选中的启动介质N	and还是SD还是其他的。

					4、 225 -- 227 执行后r2寄存器中保存启动介质
					   260 行中 r3 中赋值 #BOOT_NOR(0X03),这个在SD启动时实际会被执行，因此执行完这一段代码后r3中存储了0x03，以后备用。 

				(4) 设置栈 （SRAM中的栈）并调用 lowlevel_init 
					284 -- 286 行第一次设置栈。这次设置栈是在SRAM中设置的，因此当前整个代码还在 SRAM中运行，此时DDR还未被初始化还不能使用。站地址0xd0036000是自己指定的，指定的原则是这块空间只给栈用，不会被别人占用。
					在调用函数前初始化栈，主要原因是在被调用的函数内还有再次调用函数，而BL只会将返回地址存储到LR中，但是我们只有一个LR，所以在第二层调用函数前先将LR入栈，否则函数返回时第一层的返回地址就丢了。

					注意：给sp赋值就是设置栈

					1、 lowlevel_init 函数
						路径：uboot_kernel\X210\uboot\board\samsung\x210\lowlevel_init.S

						push	{lr}	/* 压栈 */

						(1) 检查复位状态
							复杂cpu允许多种复位情况。譬如直接冷上电、热启动、睡眠（低功耗）状态下的唤醒等，这些情况都属于复位。所以我们在复位代码中药去检测复位状态，来判断到底是哪种情况。
							判断哪种复位的意义在于：冷上电时DDR是需要初始化才能用；而热启动或者低功耗状态下的复位则不需要再次初始化DDR。
						(2) IO 状态复位
							IO Retention release 进入低功耗等复位时再次启动需要回复io

							以上两个跟主线关系不大。
						(3) 关看门狗
							参考裸机相关章节。
						(4) 一些SRAM SROM 相关GPIO设置
							与主线无关，不用管。

						(5) 供电锁存
							lowlevel_init.S的第 100-104 行，开发板供电锁存

							总结:在前100行，lowlevel_init.S中并没有做太多有意义的事（出了关看 门狗、供电锁存），然后下面从100行开始才进行有意义的操作。

						(6) 判断当前代码执行位置
							lowlevel_init.S的 110 -- 115 行
							这几行代码的作用就是判定当前代码执行的位置是在SRAM中还是DDR中。

							为什么要做这个判定？
							原因1：BL1（uboot的前一部分）在SRAM中有一份，在DDR中也有一份，因此	如果是冷启动那么当前代码应该是在SRAM中运行BL1，如果是低功耗	状态复位这时候应该就是在DDR中运行的。
							原因2：我们判定当前运行代码的地址是有用的，可以知道后面代码的运行。例	如在lowlevel_init.S中判定当前代码的运行地址，就是为了确定要		不要执行时钟初始化和初始化DDR的代码。如果当前代码是在SRAM中	，说明是冷启动，那么事中和DDR都需要初始化；如果当前代码是在DD	  R中，那么说明是热启动则始终和DDR都不用再次初始化。

分析代码：
	ldr r0, =0xff000fff   给r0加载一个常数
	bic	r1, pc, r0		将pc的值中的某些bit位清0，剩下的一些特殊bit位赋值给r1(r0中为1的那些位清零)					相当于：r1 = pc & ~(ff000fff)
	ldr	r2, _TEXT_BASE	将链接地址加载到r2中
	bic	r2, r2, r0		将r2中相应未清0，剩下特定位
	cmp     r1, r2      比较 r1 与 r2 
	beq     1f			1 标志  f 向下找

	总结：这一段代码通过读取当先运行地址和连接地址，然后处理两个地址后对比是否相等，来判断当前运行是在SRAM中（不相等）还是DDR中（相等）。从而决定是否跳过时钟和DDR。

	1、 时钟初始化 system_clock_init
		位置：当前文件的 205 行。一直到 385 行，（本地source insight的390行）
		x210_sd.h 300 - 428 行，都是和时钟相关的配置值。这些宏定义就是决定了210的时钟配置是多少。


	mov	pc, lr   函数返回。


	2、 初始化DDR mem_ctrl_asm_init
		路径：\uboot_kernel\X210\uboot\cpu\s5pc11x\s5pc110\cpu_init.S

	3、 初始化串口 uart_asm_init
		这个函数初始化串口
		初始化完，发送 O 用于初步调试

	4、 初始化 tzpc_init
		不管

	start.S 解析 7

	lowlevel_init.S中主要做了：
	1、 检查复位状态
	2、 IO回复
	3、 关看门狗					***
	4、 开发板供电锁存				***
	5、 时钟初始化				***
	6、 DDR初始化 				***
	7、 串口初始化并打印 'O'		***
	8、 tzpc初始化
	9、 打印 'K' 函数返回 		***

2021/3/8
	5、 再次设置栈(DDR中的栈)	  栈 -- 满减栈 向下走(arm)
		(1) 再次开发板供电锁存。第一，做2次是不会错误的；第二，做两次则第二次无意义；做代码移植时有一个谨慎    保守的策略就是尽量添加代码而不要删除代码。
		(2) 之前在调用lowlevel_init程序前设置过1次栈，那时候因为DDR尚未初始化，因此程序执行都是在SRAM中，所以在SRAM中分配了一部分内存作为栈。本次因为DDR已经初始化了，因此要把栈挪移到DDR中，所以要重新设置栈，这是第二次(start.S 297--299);栈：33e00000,干好在uboot的代码段的
		(3) 为什么要再次初始化栈？ DDR已经初始化了，已经有大片内存可以用了，没必要再把栈放在SRAM中可怜兮兮了；原来SRAM中内存大小空间有限，栈放在哪里要注意不能食用过多的栈否则栈会溢出，我们及时将战迁移到DDR中也是为了尽可能避免展示用时候的小心翼翼。

		感慨：uboot的启动阶段主要技巧就在于小范围内有限条件下的辗转腾挪。

	6、 在此判断当前地址以决定是否重定位
		(1) 再次用相同的代码判断运行地址是在SRAM中还是DDR中，不过本次判断的目的不同（上次判断是为了判断是否要执行 时钟、内存初始化），这次判断是为了决定是否进行uboot的relocate（重定位）。
		(2) 冷启动时当前情况是uboot的前一部分（16kb 或者 8kb）开机自动从SD卡加载到SRAM中正在运行，uboot的第二部分（其实第二部分是整个uboot）还躺在SD卡的某个扇区开头的N个扇区中。此时uboot的第一阶段已经即将结束了（第一阶段该做的事基本做完了），结束之前要把第二部分加载到DDR中拦截地址处（33e00000）,这个加载过程就是重定位。

	7、 uboot的重定位
		(1) 0xD0037488 这个内存地址在SRAM中，这个地址中的值是被硬件自动设置了的。硬件根据我们实际电路中SD卡在那个同道中，会将这个地址中的值设置为相应的数字。譬如我们从SD0通道启动时，这个值为 0xEB000000；从SD2通道启动时，这个值为 0xEB200000 (外部sd卡启动),
		(2) zai start.S的260行确定了从MMCSD启动，然后又在278行将 #BOOT_MMCSD写入INF_REG3寄存器中存储着。然后又在322行读出来，再和#BOOT_MMCSD去比较，确定是从MMCSD启动。最终跳转到mmcsd_boot函数中去执行重定位动作。

		(3) 真正的重定位是通过调用movi_bl2_copy函数完成的，在uboot/cpu/s5pc11x/movi.c中。是一个C语言的函数。
		(4) copy_bl2(2, MOVI_BL2_POS, MOVI_BL2_BLKCNT, CFG_PHY_UBOOT_BASE, 0);
			分析参数： 2 -- 表示通道2 
					  MOVI_BL2_POS -- uboot的第二部分在SD卡中的开始扇区，这个数字必须和烧录uboot时的位置相同；
					  MOVI_BL2_BLKCNT -- uboot的长度占用的扇区数；
					  CFG_PHY_UBOOT_BASE -- 重定位时将uboot的第二部分赋值到DDR中的气势地址(33E00000).

	8、 什么是虚拟地址、物理地址
		物理地址： 物理设备生产时赋予的地址，逻辑中使用的寄存器的地址就是CPU设计师指定的，这个就是物理地址		，物理地址是硬件编码的，式设计生产时确定好的，一旦确定了就不能改了。不够灵活，解决方法是		 虚拟地址。
		虚拟地址： 软件操作和硬件操作之间增加一个层次，叫做虚拟地址映射层。有了虚拟地址映射后，软件操作只需	    要给虚拟地址，硬件操作还是原来的物理地址，映射层建立一个虚拟地址到物理地址的映射表。当我		 们软件运行的时候，软件中使用的虚拟地址在映射表中查询得到对应的物理地址再发给硬件去执行。
				  （虚拟地址到物理地址的映射是不可能通过软件来实现的）

	9、 MMU 单元的作用
		(1) MMU就是memory management unit，内存管理单元。MMU实际就是SOC中一个硬件单元，主要功能就是实现虚拟地址到物理地址的映射。
		(2) MMU单元在CP15协处理器中进行控制，也就是说要操作MMU进行虚拟地址映射，方法就是对CP15协处理器的 寄存器进行编程。

	地址映射的额外收益1：访问控制（按块为单位）
		1）访问控制就是：在管理上对内存进行分块，然后每次进行独立的虚拟地址映射，然后在每一块的映射关系中同   时还实现了访问控制（对该块可读、可写、只读、只写、不可访问等控制）
		2）Segmentation fault。实际上这个段错误就和MMU实现的访问控制有关。当前程序只能操作自己有权操作的地址范围（若干个内存块），如果当前程序指针出错访问了不该访问的内存块则就会触发段错误。
	地址映射的额外收益2：cache
		1）cache的工作和虚拟地址的映射有关系。
		2）cache：快速缓存，比CPU块比DDR块。CPU嫌DDR太慢了，于是把一些DDR中常用的内容实现读取缓存在cache    中，然后CPU每次需要找东西时现在cache中找。如果cache中有就直接用cache中的；如果cache中没有才回    去DDR中寻找。 -- cache命中

		(3) 使能访问域 （CP15的c3寄存器）
			1）cp15协处理器内部有c0--c15共16个寄存器，这些寄存器每一个都有自己的作用。我们通过mrc和mcr指令来访问这些寄存器。所谓的操作cp协处理区其实就是操作cp15的这些寄存器。
			2) c3寄存器在mmu中的作用是控制域访问。域访问和mmu的访问控制有关。

		(4) 设置TTB（cp15的c2寄存器）
			1）TTB就是translation table base，转换表基地址。首先要明白什么是TT（translation table转换表），TTB就是转换表的基地址而已。
			2）转换表是建立一套虚拟地址的关键。转换表分两部分，表索引和表项（表索引：虚拟地址；表项：物理地   址），一个表索引和一个表项构成一个转换表单元，能够对一个内存块进行虚拟地址转换。（映射中基本规定了内存映射和管理事宜块为单位的，置于这个块有多大，要看你的mmu的支持和你自己的选择。在ARM中支持3中大小，细表1KB、粗表4KB、段1MB）。真正的转换表就是由若干个转换表的单元构成，每个单元负责1个内存单元，总体的转换表负责整个内存空间（0-4G）的映射。
			3）整个建立虚拟地址映射的主要工作就是建立这张转换表。
			4）转换表放置在内存中的，防止是要求起始地址在内存中要xxx位对其。转换表不需要软件去干涉使用，而是将基地址TTB设置到cp15的c3寄存器中，然后mmu工作自己回去找。

		(5) 使能MMU单元
			cp15的c1寄存器的bit0控制MMU的开关。只要将这一个bit置1即可开启MMU。开启MMU之后上层软件层的地址就必须经过TT转换才能发给下层物理层去执行。

		(6) 找到映射表
			lowlevel_init.S文件的593行

			宏观上理解转换表：整个转换表可以看做是一个int类型的数组，数组中的一个元素就是一个表索引和表项的单元。数组中的元素 值就是表项，这个元素的数组下标就是表索引。
			ARM的段氏映射中的长度为1MB，因此一个映射单元只能管1MB内存，4G需要 4G/1MB = 4096，即这个数组的元素个数是4096。实际上是是把 4096 分成几个部分，每个部分通过for循环
		
		x210 uboot 虚拟地址映射表
		VA 							PA 							len 	start -- end

		00000000 - 10000000			00000000 - 10000000			256M	0M    -- 256M
		10000000 - 20000000			00000000					256M	256M  -- 512M
		20000000 - 60000000			20000000 - 60000000			1G		512M  -- 1.5G
		60000000 - 80000000			00000000					512M 	1.5G  -- 2G 
		80000000 - b0000000			80000000 - b0000000 		768M	2G    -- 2.75G
		b0000000 - c0000000			b0000000 - c0000000 		256M	2.75G -- 3G 

		c0000000 - d0000000			30000000 - 40000000 		256M	3G    -- 3.25G
		d0000000 - 100000000 		d0000000 - 100000000 		0.75G 	3.25G -- 4G 
		
		结论：虚拟地址映射只是把虚拟地址的c0000000 开头的256M映射到了DMC0的30000000 开头的256M物理地址去了。其他的虚拟地址空间根本没动，还是原样映射的。
		思考：为什么配置时将链接地址设置为c3e00000，因为这个地址将来会映射到33e00000这个物理地址。
		
		(7) 再次设置栈 (1、sram中 2、DDR中 3、DDR中)
			1）第三次设置栈 ldr sp, =c3fff000 。 本次蛇者栈是为了吧栈放在合适地方（安全、紧凑而不浪费内存）。将栈放在uboot地址上方2MB处，这样安全的栈空间是：2MB - uboot大小 = 1.8M 左右。

		(8) 清bss段
			来源于链接脚本（u-boot.lds）

		(9) ldr	pc, _start_armboot
			1） 路径：/uboot/lib_arm/board.c。 C语言函数。 这个函数就是uboot的第二阶段。这句代码的作用就是将uboot第二阶段执行的函数的地址传给pc，实际上就是使用一个愿跳转直接跳转到DDR中的第二阶段开始地址处。

			2）远跳转的含义；这句话加载的地址和当前运行地址无关，而和链接地址有关。因此这个远跳转可以实现从SRAM中的第一阶段跳转到DDR中的第二阶段。
			3）这里这个远眺转就是uboot第一阶段和第二阶段的分界线。

		总结： uboot第一阶段做了哪些工作？
			(1) 构建异常向量表			***
			(2) 跳转到 reset 
			(3) 设置cpu特权模式			***
			(4) 失能MMU
			(5) 供电锁存
			(6) 判断启动介质
			(7) 第一次设置栈
			(8) 进入   lowlevel_init
			(9) push	{lr}	/* 压栈 因为在这里面还会调用函数，如果压栈则调用返回时会覆盖原先的信息*/
			(10) 检测复位状态(是被唤醒方式，可不用)
			(11) 低功耗唤醒时需要恢复IO
			(12) 关看门狗				***
			(13) 供电锁存，维持供电		***
			(14) 检测目前在哪运行来决定是否需要初始化clock、memory	***
			(15) 初始化uart 最后会打印 "OK"						***

			(16) 重定位 -- 将uboot整体挪移到DDR中					***
			(17) 建立映射表开启MMU								***
			(18) 设置栈											***
			(19) 跳转第二阶段										***

	2、 启动第二阶段 （_start_armboot 函数）
		1、 概述
			(1) _start_armboot 函数简介： 这个函数 + 其调用的函数 就是整个uboot第二阶段。

			(2) 宏观分析：
				概括来讲uboot第一阶段初始化了cpu内部的一些部件例如：看门狗、时钟，然后初始化DDR完成重定位。
				uboot的第二阶段就是来初始化剩下的还未初始化的外部硬件。例如：FLASH、iNAND、网卡等。uboot奔上的一些东西（uboot的命令、环境标量等）。然后最终初始化必要的东西后进入uboot的命令行准备接受命令。

			(3) 思考：uboot第二阶段完结与何处？
				(1) uboot启动后自动运行打印出很多信息（这些信息就是uboot在第一阶段和第二阶段不断进行初始化时，打印出来的信息）。然后uboot进入了倒数bootdelay秒然后执行bootcmd对应的启动命令。
				(2) 如果用户没有干涉则会执行bootcmd进入自动启动内核流程（uboot就死掉了）；此时用户可以按下回城建打断uboot的自动启动进入uboot命令行下。然后uboot就一直工作在命令下。
				(3) uboot的命令行就是一个死循环，循环体内不断重复；接收命令、解析命令、执行命令。这就是uboot最终的归宿。

		2、 分析： 
			(1) init_fnc_t 
				1）typedef int (init_fnc_t) (void); 函数类型   init_fnc_t前加个*才是函数指针

				init_fnc_ptr 是一个二重函数指针，二重指针的作用：其中一个是来指向一重指针；一个是用来指向指针数组（数组存指针）。因此这里可以用来指针一个函数指针数组。

				2）DECLARE_GLOBAL_DATA_PTR
				#define DECLARE_GLOBAL_DATA_PTR     register volatile gd_t *gd asm ("r8")
					定义了一个全局变量gd，这个全局变量是一个指针类型，占4字节。用volatile修饰表示是可变的，用register修饰表示这个变量要尽量放到寄存器中，asm("r8") 是gcc支持的一种语法，意思就是要把gd放到寄存器r8中。
					
					综合分析：DECLARE_GLOBAL_DATA_PTR 就是定义了一个要放在寄存器r8中的全局变量，名字叫gd，类型是一个指向gd_t类型变量的指针。
					
					为什么要定义为register？因为这个全局变量gd（global data简称）是uboot中很重要的一个全局变量（准确的说这个全局变量是一个结构体，里面有很多内容，这些内容加起来构成的结构体就是uboot找那个常用的所有的全局变量），这个gd在程序中经常被访问，因此放在register中提升效率。

					gd_t 中定义了很多全局变量，都是整个uboot使用的；其中有一个bd_t类型的指针，指向一个bd_t类型的变量，这是开发板的班级信息的结构体，里面有不少硬件相关的参数，譬如波特率、IP地址、机器码、DDR内存分布。

					内存使用排布：
					1、 DECLARE_GLOBAL_DATA_PTR 只是定义了一个指针，也就是gd里的这些全局变量并没有分配内存，我们是用gd之前要给他分配内存，否则gd也就是一个野指针。
					2、 gd 和 bd 需要内存，内存当前没人管理（因为没有操作系统统一管理内存），大片的DDR散放着可以随意使用（只要使用内存地址直接去访问内存即可）。 原则：安全、紧凑（需要合理规划）

					uboot区：	CFG_UBOOT_BASE - xx 	(长度为实际uboot的长度)
					堆区：	 	CFG_MALLOC_LEN			912KB
					栈区：	 	CFG_STACK_SIZE			512KB
					gd:		 	sizeof(gd_t)			36字节
					bd:		 	sizeof(bd_t)			44字节+
					内存间隔：	
						__asm__ __volatile__("": : :"memory"); //内嵌汇编 防止 >= 3.4 版本的gcc过度优化

	(2) start_armboot解析
		1）for 循环执行 init_sequence 
			init_sequence是一个函数指针数组 (一个数组里面存的全是指针变量，这些指针变量都指向函数)，这些函数都是init_fnc_t类型，其参数都是 void 返回值都是 int。
			init_sequence 在定义是同时给了初始化，初始化的函数指针都是一些函数名。

			init_fnc_ptr是一个二重函数指针，可以指向 init_sequence 这个函数指针数组。

			for 循环事项遍历这个函数指针数组，并去执行这些函数。

			如何遍历一个函数指针数组？
				第一种：用下标去遍历，用数组下标去截止
				第二种：在数组元素末尾放一个标志，遍历到该标志时结束。（类似字符串）不用统计元素个数
				此处就是使用这种方法。

				init_fnc_ptr函数，成功返回0，失败返回其他。

		init_sequence 中函数分析：
			cpu_init ： cpu相关初始化，在start.S中结束此处为空
			board_init ： board/samsung/x210/x210.c 
						  DECLARE_GLOBAL_DATA_PTR，在这里声明是为了后面使用gd方便。
						  CONFIG_DRIVER_DM9000	/* 配置网卡 uboot移植网卡相关初始化都在这
						  		主要是GPIO和端口的配置，这里都是基本初始化，都和硬件相关。*/

				机器码：  
				gd->bd->bi_arch_number = MACH_TYPE;
					bi_arch_number 开发板的机器码（开发板的唯一编号）。
					机器码的主要作用就是uboot和Linux内核之间进行比对适配，嵌入式设备的硬件都是定制化的，不能通用。

					uboot的机器码会作为参数传给Linux内核，内核将这个机器码和自己本身的比对。

				传参bootargs：
				gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100); // 0x30000100
					bi_boot_params uboot给Linux kernel 启动时的内存地址。uboot将传参（字符串：bootargs）放在 bi_boot_params处，然后启动内核。
					uboot在启动内核时真正是通过寄存器r0 r1 r2 来直接传参的

				背景：
				DDR 初始化：
							这里和汇编阶段的lowlevel_init中初始化不一样，那是硬件初始化让其可以工作；此处是软件结构中的一些DDR相关属性配置、地址设置的初始化，纯软件层面。

							软件层次初始化的原因or目的：uboot是不知道DDR的具体信息（每一片起始地址、长度等信息）的，此处就是读取配置信息中的相关配置。（PC机是通过读取相关设备信息获得信息的，uboot未采用这种方法）

			interrupt_init ： 虽然长得跟 中断 有关，但是实际上是初始化定时器4的
				1、 x210 共有5个定时器，Timer0 - Timer3都要一个对应的PWM信号输出的引脚。
					而Timer4 没有，无法输出PWM波形（没有引脚、没有TCMPB寄存器）。
				2、 Timer4用来计时，主要用到寄存器：TCNTB4、TCNTO4。TCNTB4用来计数，代表多少标准    定时（每一次是由始终决定的，其实就是由2级时钟分频器决定的）。TCNTO4用来检查T    CNTB是否减到0。
				3、 Timer4不支持中断，所以cpu只能等待并不断查看TCNTO寄存器来确定定时是否到了。

				4、 uboot就是使用Timer4（bootdelay就是这个原理）

				5、 interrupt_init此函数将Timer4设置为定时10ms。其中关键就是 get_PCLK() 获取系统设置的PCLK_PSYS时钟频率，

			env_init ： 
				1、 跟环境变量初始化有关的
				2、 有很多env_init函数是因为uboot支持很多启动介质。需要根据具体情况查找。
				3、 这个函数是对内存里维护的那一份uboot的env做了基本的初始化或者是判定（是否可用   ）。当前因为我们还没有进行环境变量从SD卡到DDR中的relocate，因此当前环境变量时   不能用的。
				4、 在start_armboot函数的776行调用，env_relocate才进行环境变量从SD卡中到DDR中    的重定位。重定位之后需要环境变量时才可以从DDR中去取，重定位之前如果要用只能去S   D卡中去读。

			init_baudrate ：
				1、 初始化串口通信得波特率
				2、 getenv_r函数读取环境变量"baudrate"的值，这个值是字符串然后用simple_strtoul
				   函数转换为数字。
				3、 baudrate初始化的规则是：先去环境变量中读取"baudrate"这个环境变量的值。如果读取成功则使用这个值作为环境变量，记录在gd->baudrate和gd—>bd->bi_baudrate中；如果不成功则使用x210_sd.h中的CONFIG_BAUDRATE的值作为波特率。
			serial_init ： 
				1、 初始化串口
				2、 疑问：在start.S中调用lowlevel_init.S中已经使用汇编初始化串口了，这里为什么又
				   初始化？这两个初始化是重复还是各自有不同？

				   其实啥都没做，只用了for去延时。第一阶段已经初始化过了。

			console_init_f：
				1、 console（控制台）的第一阶段初始化。 _f表示第一阶段初始化，_r表示第二阶段初始化。中间还要做一些其他的。console_init_r 在826行。
				2、 仅仅将 gd->have_console = 1; 因此目前控制台还不能用。

			display_banner：
				1、 显示uboot的版本信息 logo
				2、 display_banner 使用printf函数向串口输出了version_string这个字符串。
				3、 由上面的console_init_f函数可知目前并没有初始化号console，但为什么可以打印了呢？
					通过追踪发现，printf -> puts,而puts函数中会判断当前uboot中console有没有初始化好。如果console初始化好了则调用fputs完成串口发送；如果console没有初始化好则会调用serial_puts（在调用serial_putc直接操作串口）
				4、 console也是通过串口输出，非控制台也是通过串口输出，这有什么区别？
					实际上在uboot里面console就是用软件虚拟出来的设备，这个设备有一套专用的通信函数（发送、接收...），控制台的通信函数最终会映射到硬件的通信函数中来实现的。uboot中实际上控制台的通信函数是直接映射到硬件串口的通信函数中的，也就是说uboot中用没用控制台其实并没有本质差别。

				version_string：
				  U_BOOT_VERSION 在uboot 源码中没有定义，是在主Makefile中拼接并导出到一个文件中，

			print_cpuinfo：
				1、 打印CPU配置（时钟）信息。

			checkboard：
				1、 检查开发板，并打印出开发板名字。

			init_func_i2c
				1、 此函数未执行。

		uboot实践：
			1、 对uboot源代码进行修改（根据自己的理解修改）
			2、 make distclean -> make x210_sd_config -> make
			3、 便已完成得到u-boot.bin，然后烧录。烧录方法按照逻辑第三部分的Linux下使用dd命令来烧写的方法来烧写。
			4、 烧写过程：
				(1) 进入sd_fusing目录下
				(2) make clean
				(3) make
				(4) 插入sd卡
					查看/dev/sd* 新生成的设备号。
				(5) ./sd_fusing.sh /dev/sdb完成烧录

			dram_init:
				1、 关于DDR的初始化。疑问：汇编阶段已经初始化过DDR了否则也无法relocate到第二阶段运行 ，怎么在这里有初始化DDR了？
					经分析这里只是软件层面的初始化，即给gd->bd里面关于DDR配置部分的全局变量赋值，让gd->bd数据记录下当前开发板的DDR的配置信息。

			display_dram_config：
				1、 打印DRAM的配置信息
				2、 思考：如何在uboot运行中得知uboot的DDR的配置信息？
					uboot中有一个命令叫bdinfo，这个命令可以打印出gd->bd中记录的所有硬件相关的全局变量的
					值，因此可以得知DDR的配置信息。

		init_sequence总结：
			(1) cpu_init 空
			(2) board_init 网卡初始化、机器码 gd->bd->bi_arch_number、gd->bd->bi_boot_params
			(3) interrupt_init 初始化定时器Timer4 10ms一次
			(4) env_init 无实质性东西
			(5) init_baudrate 初始化波特率，设置波特率
			(6) serial_init 空
			(7) console_init_f 控制台初始化第一阶段，只是 gd->have_console = 1;
			(8) display_banner 打印uboot的启动信息，就只有一句
			(9) print_cpuinfo 打印cpu相关的信息，主要是频率	
			(10) checkboard 打印一句 x210
			(11) init_func_i2c 没有运行
			(12) dram_init 初始化DDR，将相关信息写到gd里面
			(13) display_dram_config 打印DDR信息

		2）CFG_NO_FLASH
			1、 虽然NandFlash和NorFlash都是Flash，但是一般NandFlash会简称Nand而不是Flash，一般将Flash都是指NorF   lash。这里的两行代码是NorFlash相关的。
			2、 flash_init执行的事开发板中NorFlash的信息。

		3）CONFIG_VFD和CONFIG_LCD
			1、 uboot自带的LCD显示的软件架构。但实际上我们用的LCD没有使用uboot中设置的这套软件架构，我们在后面自己   添加了一个LCD显示部分。
		4）mem_malloc_init
			1、 mem_malloc_init用来初始化uboot的堆管理器，uboot中自己维护了一段堆内存，肯定自己就有一套代码来管理这个堆内存。有了这些东西uboot中你也可以malloc、free这套机制来申请内存和释放内存。我们在DDR内存中给堆预留了896KB的内存。

		5） x210的相关代码在 599 - 632 行
			1、 mmc_initialize mmc相关的初始化，其实就是用来初始化SOC内部的SD/MMC控制器的，

			2、 mmc_initialize是具体硬件架构无关的一个MMC初始化函数，所有的使用了这套建构的代码都调用这个函数来完   成MMC的初始化。mmc_initialize中在调用board_mmc_init和cpu_mmc_init来完成具体的硬件的MMC控制器的    初始化。
			3、 cpu_mmc_init 在uboot/cpu/s5pc11x/cpu.c中，这里面有吊用了drivers/mmc/s3c_mmcxxx.c中的驱动代码来初始化硬件MMC控制器。

		6） env_relocate 环境变量重定位
			1、 完成从SD卡中读取环境变量到DDR中。
			2、 环境变量到底从哪里来？SD卡中有一些（8个）独立的扇区作为环境变量存储区域的。但是我们烧录/部署系统时 ，我们只是烧录了uboot分区、kernel分区和rootfs分区，根本不曾烧录env分区。所以当我们少录完系统第一次启动时ENV分区是空的，本次启动uboot尝试去SD卡的ENV分区读取环境变量时失败（读取回来后进行CRC校验失败），我们uboot选择从uboot内部代码中设置的一套默认的环境变量出发来使用（默认环境变量）；这套磨人的环境变量会被读取到DDR中的环境变量中，然后被写入（也可能是saveenv时写入，也可能是uboot设计了第一次读取默认环境变量后就写入）SD卡的ENV分区。下次开机是uboot机会从SD卡的ENV分区读取环境变量到DDR中，这次读取就不会
			失败了。
			3、真正的从SD卡到DDR中重定位在 env_relocate_spec 函数中的 movi_read_env。
		7）ipaddr
			1、 开发板中的IP地址是在gd->bd中维护的，来源于环境变量ipaddr。
			2、 getenv用来获取字符串格式的ip地址。
			3、 string_to_ip用来将字符串的ip地址转换为点分十进制格式。

		8）devices_init
			1、 devices_init设备初始化。这里的设备指的就是开发板上的硬件设备。放在这里初始化的设备都是驱动设备，这个函数本来就是从驱动框架中衍生出来的。此处用来初始化从Linux内核中继承来的驱动。
		9）jumptable_init
			1、 jumptable跳转表，本身是一个函数指针数组，里面记录了很多函数的函数名。
			2、 实现一个函数指针到具体函数的映射表，将来通过跳转表中的函数指针就可以执行具体的函数。这个就是在用C语言实现面向对象编程。在Linux内核中有很多这种技巧。
			3、 通过分析这个跳转表只是被赋值从未被引用。
		10）console_init_r
			1、 console_init_f是控制台第一阶段初始化，第一阶段没做什么。第二阶段才做了实际的工作
			2、 学会找到正确的函数
			3、 console_init_r就是控制台纯软件建构的初始化（说白了就是去给console相关的数据结构中填充相应的值），所以属于纯软件配置类型的初始化。
			4、 uboot的console实际上并没有干有意义的转化，他就是直接调用的串口通信的函数。所以用不用console实际并没有什么区别。（在Linux内console就可以提供缓冲机制等不用console不能实现的功能）。

		11）enable_interrupts   __asm__ 内嵌汇编
			1、 中断初始化，使能cpu中总中断
			2、 因为我们uboot中没有使能中断，没有定义CONFIG_USE_IRQ宏，所以该函数不做任何操作。
			3、 根据宏实现条件编译。在调用处条件编译、在函数实现出条件编译。
		12）loadaddr、bootfile两个环境变量
			1、 这两个都是内核启动有关的，在启动Linux内核时会参考这两个环境变量的值。
		13）BOARD_LATE_INIT
			1、 board_late_init这个函数在这里并没有做什么操作，是空的。
			2、 board_late_init这个函数是开发版级别的一些初始化里比较晚的了，就是晚期初始化。所以晚期就是前面该初始化的都初始化了，剩下的一些必须放在后面初始化的就在这里了。
		14）eth_initialize
			1、 网卡相关初始化 ，这里不是soc与网卡芯片连接时soc这边的初始化，而是网卡芯片本身的一些初始化。
			2、 miiphy 就是网卡初始化的一个分层
			3、 X210（DM9000）初始化在board_init中。
		15）x210_preboot_init
			1、 fb相关的初始化，显示x210的logo。
		16）check_menu_update_from_sd
			1、 在uboot启动的最后阶段设置了一个自动更新的功能。 就是：我们可以将要升级的镜像放在特定的目录下，然后开机时在uboot启动的最后阶段检查升级标志（是一个按键。按键中标志位"LEFT"的那个按键，这个按键如果按下则表示updata mode，如果启动时为按下则表示normal mode）。如果进入updata mode则uboot会自动从SD卡读取镜像文件然后烧录到iNand中；如果进入boot mode则uboot不执行updata，直接启动。
			2、 这种机制能够帮助我们快速烧录系统，常用于量产时用SD卡进行系统烧录部署。

		17）main_loop
			1、 解析器 
			2、 开机倒数自动执行
			3、 命令自动补全

	uboot启动第二阶段总结：
		1、 启动流程回顾、重点函数标出
			主要是对开发板级别、软件架构级别的初始化。
			init_sequence
				cpu_init 				空
				board_init 				网卡、机器码、传参
					dm9000_pre_init();
					gd->bd->bi_arch_number = MACH_TYPE;
					gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);
				interrupt_init 			定时器Timer4
				env_init 				没做实际
				init_baudrate			波特率修改
				serial_init				没做实际的事 就是一个for延时
				console_init_f 			gd->have_console = 1;
				display_banner 			显示uboot的版本信息
				print_cpuinfo 			打印CPU配置（时钟）信息。
				checkboard 				打印了x210
				init_func_i2c 			没做实际操作
				dram_init				初始化DDR，将相关信息写到gd里面	
				display_dram_config 	打印DDR信息
			
			mem_malloc_init 			初始化堆管理器
			mmc_initialize 				inand/sd卡初始化
			env_relocate 				环境变量重定位
			gd->bd->bi_ip_addr 			ipaddr赋值
			gd->bd->bi_enetaddr 		gd数据结构赋值
			devices_init 				空的
			jumptable_init 				不用关注
			console_init_r 				控制台初始化 
			enable_interrupts           空
			loadaddr、bootfile 			环境变量读出然后初始化全局变量
			board_late_init     		空
			eth_initialize 				空
			x210_preboot_init 			LCD初始化fb显示logo 
			check_menu_update_from_sd 	uboot模式、检查自动更新
			main_loop 					循环命令解析、跳转Linux内核

		2、 启动过程特征总结
			(1) 第一阶段汇编阶段、第二阶段为c阶段
			(2) 第一阶段在SRAM中、第二阶段在DRAM中
			(3) 第一阶段注重SoC内部、第二阶段注重SoC外部Board内部

		3、 移植时的注意点
			(1) x210_sd.h头文件中的宏
			(2) 特定硬件的初始化函数位置（譬如网卡）

	启动第三部分：uboot如何启动内核
		1、 uboot和内核到底是什么
			(1) uboot是一个裸机程序
				本质：复杂的裸机程序
				ARM裸机写的简单shell，就是一个mini型的uboot。
			(2) 内核本身也是一个"裸机程序"
				本质：复杂的裸机程序
				区别：操作系统运行起来后，软件上被分为了内核层和用户层。分层后两层的权限不同，内存访问和设备管理上更加精细（内核可以随便访问各种硬件，而应用程序只能被限制的访问硬件和内存地址）。

				直观理解：uboot的镜像是u-boot.bin，Linux系统的镜像是zImage，这两个都是裸机程序镜像。从系统启动角度来讲，内核就是一个大的复杂的裸机程序。

			(3) 部署在SD卡特定分区
				一个完整的软件+硬件的嵌入式系统，静止时(未上电)BootLoader、kernel、rootfs等必须的软件都已镜像的形式存储在启动介质中（x210中是iNand/SD卡）；运行时都是在DDR内存中运行的，与存储介质无关。以上两个状态都是稳定状态，第三个状态是一个动态过程，即从静止态到运行态的过程，也就是启动过程。

				动态启动过程就是一个从SD卡逐步搬移到DDR内存，并运行启动代码进行相关硬件初始化和软件架构的建立，最终运行时稳定状态

				静止时u-boot.bin zImage rootfs都在SD卡中，他们不能随意存储，因此需要对SD卡分区，然后各种镜像各自存在各自的分区中，这样启动过程中uboot、内核就知道去哪里找。（uboot和kernel中的分区标必须一致，同时和SD卡的实际使用要一直）

			(4) 运行时必须先加载到DDR中连接地址处
				uboot在第一阶段中进行重定位时将第二阶段（整个uboot镜像）加载到DDR的0xc3e00000处，这个地址是uboot的链接地址。
				内核也有类似的要求，uboot启动内核时将内存从SD卡读取放到DDR中（其实就是个重定位），不能随意放置，必须放在内核的链接地址处，否则启动不起来。

			(5) 内核启动需要必要的启动参数
				uboot是无条件启动的，从零开始。
				内核是不能开机自动完全从零开始启动的，内核启动要别人帮忙。uboot要帮助内核实现重定位（从SD卡到DDR），uboot还要给内核提供启动参数。

		2、 启动内核第一步
			(1) uboot启动内核分为两部分：
				第一步是将内核镜像从启动介质中加载到DDR中。
				第二步失去DDR中启动内核镜像。

			(2) 静态内核镜像在哪里？
				1）SD卡/iNand/Nand/NorFlash等：raw分区
					常规启动时各种镜像都在启动介质中，因此uboot只需要从SD卡的kernel分区去读取内核镜像到DDR中即可。读取要是用uboot的命令	（例如x210的iNand版本是movi命令，x210的Nand版本及时Nand命令）
					这种启动方式来加载ddr，使用命令：movi read kernel 30008000。其中kernel指的是uboot中的kernel分区（就是uboot中规定	的SD卡中的一个区域范围，这个区域范围被设计来存放kernel镜像）。

				2）tftp、nfs等网络下载方式从远端服务器获取镜像
					uboot还支持远程启动，也就是内核镜像不放在SD卡中，而是放在主机服务器中，然后需要启动时uboot通过网络从服务器中下载镜像到开发板的DDR中。

				分析总结：最终的结果是内核镜像到DDR中特定地址即可，不管内核镜像是怎么到DDR中。以上2中方式各有优劣。产品出厂时设置为SD卡启动（客户不会搭建tftp服务器...），tftp下载远程启动这种方式一般用来开发。

			(3) 镜像要放在DDR的什么地址？
				内核一定要放在链接地址处，链接地址去内核源代码的连接脚本或者Makefile中去查找。

		3、 zImage和uImage的区别联系。
			(1) bootm命令对应do_bootm函数
				1）命令名前加 do_ 即可构成这个命令对应的函数，因此当我们bootm命令执行时，uboot实际执行的函数叫do_bootm函数，在cmd_bootm.c中。
				2）do_bootm刚开始定义了一些变量，然后用宏来条件编译执行了secureboot的一些代码（主要进行签名认证），先不管他；然后进行了   一些细节操作，也不管；然后到了CONFIG_ZIMAGE_BOOT，用这个宏来控制进行条件编译一段代码，这段代码是用来只是zImage格式    的内核启动的。

			(2) vmlinuz和zImage和uImage
				1）uboot经过编译生成elf的可执行程序是u-boot，相当于.exe程序，elf是可以直接执行的，但是这种格式不能用来烧录下载。我们用来烧录下载得失u-boot.bin，这个东西是由u-boot使用arm-linux-objcopy工具加工（去掉一些无用的）得到的。u-boot.bin就叫镜像（image），景象就是用来烧录iNand中执行的。

					u-boot - arm-linux-objcopy -> u-boot.bin

				2）Linux内核经过编译后也会生成一个elf格式的可执行程序，叫vmlinux或vmlinuz，这个就是原始的未经任何处理的原版的内核elf文   件；嵌入式在部署时烧录的不是vmlinuz而是用arm-Linux-objcopy工具去制作成烧录镜像格式（就是u-boot.bin这种，但是内核没有.bin后缀），经过制作加工成烧录镜像的文件就叫Image（制作把78M大的精简到7.5M，因此这个制作烧录镜像主要目的就是缩小大小，节约磁盘）。
				3）原则上Image即可以直接烧录到Flash上进行启动执行（类似于u-boot.bin），但是实际上并不是这么简单，而是对Image进行了压缩	，并且在image压缩后的文件的前端附加了一部分解压缩代码（自解压）。构成了一个压缩格式的镜像就叫zImage。（因为当年Im	age的大小刚好比一张软盘（1.2M、1.44M）大，因此设计了这种压缩Image成zImage的技术）。

				4）uboot为了启动内核Linux内核，还发明了一种内核格式uImage。uImage是由zImage加工得到的，uboot中有一个工具可以将zImage	加工成uImage。注意：uImage不关内核的事，Linux内核直观生成zImage即可，然后uboot中的mkimage工具再去将zImage生成	uImage来给uboot启动。这个加工过程其实就是在zImage前面加上64字节的uImage的头信息即可。
				5）原则上uboot启动应该给它uImage格式的内核镜像，但是实际上uboot中也可以支持zImage，是否支持就看x210_sd.h中是否定义了CONFIG_ZIMAGE_BOOT宏。

			(3) 编译内核得到uImage去启动
				1）如果直接kernel地下去make uImage会提示mkimage command not found。解决办法是去uboot/tools下cp mkimage /usr/local/bin/，赋值mkimage工具到系统目录下。

		4、 zImage启动细节
			(1) do_bootm中一直到397行的after_header_check这个符号处，都是在进行镜像的头部信息校验。校验时就要根据不同类型的image进行不    同的校验。do_bootm函数的核心就是分辨传进来的image是什么类型，然后按照这种信息格式去校验。

			(2) LINUX_ZIMAGE_MAGIC
				这个是一个魔数，这个数等于 0x016f2818 ，表示这个镜像是一个zImage。也就是说zImage格式的镜像中在头部的一个固定位置存放了这个数作为格式标记。如果我们拿到了一个image，去他的那个位置去取4字节判断他是否等于 LINUX_ZIMAGE_MAGIC ，则可以知道这个镜像是不是一个zImage。

			(3) bootm 0x30008000, 所以bootm的argc = 2，argv[1] = "0x30008000"

			(4) zImage头部开始第 37 -- 40 字节处存放着zImage标识魔数，从这个位置取出然后对比 LINUX_ZIMAGE_MAGIC。可以用sublime Text打开对比。

			(5) image_header_t
				这个是zImage启动过程中一个标准的启动数据结构，zImage头信息也是一个 image_header_t ,但是在实际启动之前需要进行一些改造。hdr->ih_os = IH_OS_LINUX; hdr->ih_ep = ntohl(addr); 这两句就是对其进行改造。

				images这个全局变量时do_bootm函数中使用，用来完成启动过程的。zImage的校验过承诺其实就是先确认是不是zImage，然后在修改zImage的头信息到合适，修改后用头信息去初始化images这个全局变量，然后完成了校验。

		5、 uImage启动细节
			(1) uImage穹顶
				1）LEGACY（遗留的），在do_bootm函数中，这种方式指的是uImage的方式。
				2）uImage方式是uboot本身发明的支持Linux启动的镜像格式，但是后来这种方式本证明是不好的，所以被新的方式替代了。这个新的方式就是 设备树 方式（在do_bootm方式中叫 FIT ）
			(2) uImage的启动校验主要在 boot_get_kernel 函数中，主要任务就是校验uImage的头信息，并且得到真正的kernel的起始位置去启动。
			(3) 
			(4) 
			(5) 
		6、 设备树方式内核启动
			(1) 设备树方式启动暂时不讲，后期补充章节将。 

		总结1：uboot本身设计时支持吃uImage启动，后来有了FDT方式之后，就降uImage方式命令为LEGACY方式，fdt方式命令为FIT方式，于是乎多谢	了#if ... #endif添加的代码。后来移植的人又为了省事添加了zImage启动的方式，又为了省事吧zImage启动方式直接写在了uImage和f	dt启动方式之前，于是乎又有了一堆#if ... #endif。 
		总结2：第二阶段校验头信息结束，下面进行第三阶段，第三阶段主要是启动内核，调用 do_bootm_linux 函数来完成。

		7、 do_bootm_linux函数
			(1) 路径：uboot/libarm/bootm.c中
			(2) 镜像entrypoint
				1）ep就是 entrypoint 的缩写，就是程序的入口。一个镜像文件的起始执行部分不是在镜像的开头（镜像开头有n个字节的头信息），真	正的镜像文件执行的第一句代码在镜像的中部某个字节处，相当于头信息是由一定的偏移量的。这个偏移量记录在头信息中。
				2）一般执行一个镜像都是：第一步先读取头信息，然后在头信息的特定地址找 MAGIC_NUM,由此来确定镜像种类；第二部对镜像进行校验；第三部再次读取头信息，有特定地址知道这个镜像的各种信息（镜像长度、镜像种类、入口地址）；第四步去entrypoit处执行镜像。
				3）theKernel = (void (*)(int, int, uint))ep;
					将ep赋值给thekernel，则这个函数指向就是指向了内存中加载的OS镜像的真正入口地址（就是操作系统的第一句执行的代码）。
			
			(3) 机器码的再次确定
				1）uboot在启动内核时，机器码要传给内核。uboot传给内核的机器码是怎么确定的？第一顺序备选是环境变量machid，第二顺序备选是gd->bd->bi_arch_num（x210_sd.h中硬编码配置的）
			
			(4) 传参并启动概述
				1）110 -- 144 行就是uboot再给Linux内核准备传递的参数处理。
				2）Starting kernel ... 这是uboot中最后打印出来的东西，只要他出现说明uboot是成功的，镜像加载、校验成功，也找到入口地址了，也试图去执行了。如果这句后串口没有输出了说明内核并没有成功执行。原因一般是：传参（80%）、内核在DDR中的加载地址有问题...。
		8、 传参详解
			(1) tag方式传参
				1）struct tag。tag是一个数据结构。在uboot和kernel中都有且一样。

				2）tag_header和tag_xxx。 tag_header中有这个tag的size和类型编码，kernel拿到一个tag后先分析tag_header得到tag的类型和大小，然后将tag中剩余部分当做一个tag_xxx来处理。

				3）tag_start与tag_end。 kernel接收到的传参是若干个tag构成的，这些tag有tag_start起始，到tag_end结束。

				4）tag传参的方式是Linux kernel发明的，kernel定义了这种想我传参的方式，uboot只是实现了这种传参。

			(2) x210_sd.h中配置传参宏
				1）CONFIG_SETUP_MEMORY_TAGS 。 tag_mem，传参内容时内存配置信息。
				2）CONFIG_CMDLINE_TAG 。 tag_cmdline, 传参内容是启动命令行参数，也就是uboot环境变量的bootargs。
				3）CONFIG_INITRD_TAG 。 tag_initrd， 
				4）CONFIG_MTDPARTITION 。 分区表iNand/SD卡的分区表。

				5）起始tag是ATAG_CORE、结束tag是ATAG_NONE,其他的ATAG_XXX都是有效信息tag。

			思考：内核如何拿到这些tag？
				uboot最终调用theKernel函数来执行Linux内核的，uboot调用这个函数（起始就是Linux内核）时传递3个参数。这三个参数就是iuboot直接传递给Linux内核的3个参数，通过寄存器来实现传参的。（第一个参数就放在r0，第二个参数放在r1，第三个参数放在r2中）第一个参数固定为0，第二个参数是机器码，第3个参数传递的就是大片传参tag的首地址。

			(3) 移植时注意事项
				1）uboot移植时一般只需要配置相应的宏即可
				2）kernel启动不成功，注意传参是否成功。传参不成功首先看uboot中bootargs设置是否正确，其次看uboot是否开启了相应宏以支持传参。

		9、 uboot启动内核的总结
			(1) 将内核搬移到DDR中
				iNand/SD卡/远端服务器 -- > DDR的链接地址
			(2) 校验内核格式、CRC等
				格式： zImage、uImage...
			(3) 传参
				uboot准备
			(4) 跳转执行
				函数指针
			(5) 涉及到的主要函数是：do_bootm和do_bootm_linux
				do_bootm： 内核搬移、校验
				do_bootm_linux：准备传参、跳转执行
			(6) uboot能启动的内核格式：zImage、uImage、fdt
				zImage： 自解压代码 + vmlinux/zmlinux压缩
				uImage： 头部信息 + zImage 
				fdt：    设备树
			(7) 跳转与函数指针的方式运行内核

	第四部分：uboot命令体系
		初步分析：
			(1)、 使用uboot命令
			(2)、 uboot命令体系实现代码在哪里
				uboot命令体系的实现代码在uboot/common/cmd_xxx.c中。有若干个.c文件和命令体系有关。（还有command.c main.c也和命令有关）
			(3)、 每个命令对应一个函数
				需要了解每一个命令背后对应的那个函数，而且要分析这个函数和这个命令是怎么对应起来的。
			(4)、 命令参数以argc、argv形式存在。
				uboot命令体系支持传参。

			举例分析：以help命令为例
				help命令背后对应的函数名叫：do_help。	int do_help (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])

		1、 从main_loop开始
			(1) uboot启动的第二极端，在初始化了所有该初始化的东西后，进入了一个死循环，死循环的循环体及时main_loop。

			(2) run_aommand函数就是用来执行命令的函数
				1）控制台命令获取
					readline函数读取命令行输入的命令字符串。
				2）命令解析
					parse_line函数吧"md 30000000 10"解析成argv[0] = md, argv[1] = 30000000, argv[2] = 10;
				3）命令集中查找命令
					find_cmd:去uboot函数集合去搜索是否存在argv[0]这个命令
				4）执行命令
					函数指针的方式调用执行对应的函数。

				总结：关键在于find_cmd函数如何查找到这个命令是不是uboot的合法支持的命令？
					这取决于uboot的命令体系机制（uboot是如何完成命令的这一套设计的，命令如何去注册、存储、管理、索引）。

			(3) uboot如何处理命令集
				1）可能的管理方式
					数组。 
						结构体数组，数组中每一个结构体成员就是一个命令的所有信息。 （数组不灵活）
					链表
						链表的每个节点data段就是一个命令结构体，所有的命令都放在一跳链表上。这就解决了数组方式的不灵活。坏处是需要额外的内存开销，然后各种算法（遍历、插入、删除等）需要一定复杂度的代码执行。
					有第三种吗？
						uboot没有使用数组或者链表，而是使用了一种很少接触的方式来实现这个功能。

				2）命令结构体cmd_tbl_t
					struct cmd_tbl_s {
						char	*name;		/* Command Name		命令名字	*/
						int		maxargs;	/* maximum number of arguments  参数最大个数	*/
						int		repeatable;	/* autorepeat allowed?	什么都不输入直接回车，执行最后的命令*/
										/* Implementation function	*/
						int		(*cmd)(struct cmd_tbl_s *, int, int, char *[]); /*函数指针，对应命令的实际操作*/
						char		*usage;		/* Usage message	(short)	*/
					#ifdef	CFG_LONGHELP
						char		*help;		/* Help  message	(long)	*/
					#endif
					#ifdef CONFIG_AUTO_COMPLETE
						/* do auto completion on the arguments */
						int		(*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
					#endif
					};
					typedef struct cmd_tbl_s	cmd_tbl_t;

					(1) name : 命令名字
					(2) maxargs : 参数最大个数
					(3) repeatable : 什么都不输入直接回车，执行最后的命令,uboot命令体系的一种机制
					(4) cmd : 函数指针，对应命令的实际操作
					(5) usage : 命令简单描述
					(6) help : 命令详细信息
					(7) complete : 函数指针，这个命令自动补全。

					总结：uboot的命令体系在工作时，一个命令对应一个 cmd_tbl_t 结构体的一个实例，然后uboot支持多少个命令，就需要多少个命令实例。uboot的命令体系把这些结构体管理起来，当用户输入一个命令时，uboot回去这些结构体实例中查找（查找方法和存储管理的方法有关）。如果找到则执行命令，找不到则提示退出。

				3）uboot实现命令管理的思路
					(1) 填充1个结构体构成一个命令
					(2) 给命令结构体实例附加特定属性（用户自定义段），连接时降低阿尤该属性的内容连接在一起排列。（挨着的，不会夹杂其他东西，也不会丢到一个带有这种段属性的，但是顺序是乱的）。
					(3) uboot重定位时将该段整体加载到DDR中。加载到DDR中的uboot镜像中带有特定段属性的这一段其实就是命令结构体的集合，有点像一个命令结构体数组。
					(4) 段起始地址和结束地址（链接地址、定义在u-boot.lds中）决定了这些命令集的开始和结束地址。

				4）uboot如何处理命令集2
					(1) U_BOOT_CMD
						这个宏定义在uboot/commom/command.h中。
						#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
								cmd_tbl_t __u_boot_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage, help}
								#name ： 将name变成一个字符串

						示例如下：
						U_BOOT_CMD(
							version,	1,		1,	do_version,
							"version - print monitor version\n",
							NULL
						); 
						--->
						cmd_tbl_t __u_boot_cmd_version __attribute__ ((unused,section (".u_boot_cmd"))) \
						 = {#name, maxargs, rep, cmd, usage, help}

						 __attribute__ ((unused,section (".u_boot_cmd")))  ： gcc的一种扩展语法，给前面的贴一个标签
						 .u_boot_cmd ： 链接脚本中一个用户自定义段

						 总结： U_BOOT_CMD宏的理解，关键在于结构体变量的名字和段属性。名字使用 ## 作为连字符。段属性理解：就是给它贴一个标签。附加了用户自定义的段属性，以保证连接时这些结构体紧挨着的。
					(2) 链接脚本 

					(3) find_cmd函数详解
						从当前uboot的命令集中查找是否有这个命令，返回这个命令对应的结构体的指针。实现思路很简单，如果不考虑带.的就更简单了，查找命令的思路就是for循环比那里数组的思路，不同的是数组的起始地址和结束地址是用地址值来给定的，数组中的元素是结构体变量类型。
					(4) U_BOOT_CMD宏详解
						这个宏起始就是定义了一个命令对应的结构体变量，这个变量名和宏的第一个参数有关，因此只要宏调用时传参的第一个参数不同则定义的结构体变量不会重名。

				3）添加uboot命令
					(1) 直接在现有command.c文件中添加命令，很简单，直接复制现有的进行更改即可。
					(2) 自建一个.c文件并添加
						1）在uboot/common目录下新建一个命令文件，叫cmd_aston.c（对应的命令名字叫aston，对应的函数就叫do_aston函数），然后在c文件中添加命令对应的U_BOOT_CMD宏和函数。注意头文件包含。

						2）uboot/common目录下的Makefile中添加。

	第五部分：环境变量
		1、 uboot环境变量基础
			(1) 环境变量的作用
				让我们可以不用修改uboot代码，就可以更改uboot的运行。
			(2) 环境变量的优先级
				uboot代码当中有一个值，环境变量中有一个值，优先使用环境变量的值。
				例如：machid  do_bootm_linux函数94行
							s = getenv ("machid");
							if (s) {
								machid = simple_strtoul (s, NULL, 16);
								printf ("Using machid 0x%x from environment\n", machid);
							}
			(3) 环境变量在uboot中工作方式
				1）默认环境变量，在uboot/common/env_common.c中default_environment
					这东西本质是字符数组，大小为CFG_ENV_SIZE（16KB），里面内容就是很多个环境变量连续分布组成的，每个环境变量最末端以'\0'结束。

				2）SD卡中环境变量分区，在uboot的raw分区中
					SD卡其实就是给个分区，专门用来存储而已。存储时其实是把DDR中的环境变量整体的写入SD卡中分区里。所以当我saveenv时其实整个所有的环境变量都被保存了一遍，而不是只保存更改了的。

				3）DDR中环境变量，在default_environment中，实质是字符数组。
					在uboot中其实一个全局变量，连接时在数据段，重定位时default_environment就被重定位到DDR中的一个地址处了。这个地址的全局字符数组就是我们uboot运行时的DDR中的环境变量了。

				总结：刚烧录的系统中环境变量分区是空白的，uboot第一次运行时加载得是uboot代码中自带的一份环境变量，叫默认环境变量。我们在saveenv时DDR钟的环境变量会被更新到SD卡中的环境变量中，就可以被保存下来，下次开机会在环境变量relocate时会将SD卡中的环境变量会被加载到DDR中去。

				default_environment中的内容虽然被uboot源代码初始化为一定的值（这个值就是我们的默认环境变量），但是在uboot启动的第二阶段，env_relocate时代码回去判断SD卡中的env分区的crc是否通过。如果crc校验通过说明SD卡中有正确的环境变量存储，则relocate函数会从SD卡中读取环境变量来覆盖default_environment字符数组，从而每次开机可以保持一次更改过的环境变量。

			(4) printenv
				环境变量源函数 -- > do_printenv

				分析do_printenv函数：

				环境变量在内存中怎么存储？
					按照字符数组存储在内存中。	"bootargs="	CONFIG_BOOTARGS	"\0"   
											/*"bootargs=“ 与 CONFIG_BOOTARGS 会合并在一起，“bootargs=CONFIG_BOOTARGS\0 ”*/
			(5) setenv
				函数： setenv --> do_setenv --> _do_setenv （函数有下划线：没有下划线的是自己可以弄得、有一个下划线的事系统用的、有两个下划线的事系统内部用的）
				
			(6) saveenv
				1）在/uboot/common/cmd_nvedit.c中。 do_saveenv
				2）从uboot实际的输出以及x210_sd.h中可以分析出我们实际使用的事env_auto.c中（#define CFG_ENV_IS_IN_AUTO）。env_name_spec
				3）env_auto.c中使用宏定义的方式条件编译了各种常见的flash芯片。在程序中读取INF_REG（OMpin 内部对应的寄存器）从而知道我们的启动介质。

				4）INF_REG3_REG = 0xE010F000 + 0x03,含义是的用户自定义数据。我们在start.S中判断启动介质时写入的 3 ，所以执行函数是saveenv_movinand

				5）最后执行的事movi_write_env函数
				6）raw_area_control是uboot中规划iNand/SD卡的原始分区表，这里面记录了我们对iNand的分区，env分区也在这里，下标2.追到这里就可以了，下面就是底层驱动了。

			(7) uboot内部获取环境变量
				1）getenv	不可重入（使用了全局变量、静态局部变量）
					实现方法：遍历default_environment数组，挨个拿出所有的环境变量比对name，知道相等的直接返回这个变量的首地址即可。

				2）getenv_r 可重入
					返回找到环境变量然后 copy到提供的buff中。

				总结：功能一样，getenv_r安全一些。
					  理解环境变量在DDR中的存储方法，理解了环境变量和gd全局变量的关联和优先级，理解了换将变量在存储介质中的存储方式（专用raw分区），整个换将变量相关的就清楚了。

	第六部分：硬件驱动
		1、uboot与Linux驱动
			1）uboot本身是逻辑程序
				(1) 裸机本来是没有驱动的概念的（狭义的驱动的概念就是操作系统中用来具体操控硬件的那部分代码叫驱动）
				(2) 裸机程序是直接操控硬件的。操作系统中是必须通过硬件来操控硬件的。这两个有什么区别？
					没有本质区别，主要区别在分层。
			2）uboot的虚拟地址对硬件操作的影响
				(1) 操作系统（Linux）下MMU肯定是开启的，也就是说Linux驱动中肯定都使用的事虚拟地址。而纯裸机程序中根本不会开MMU，全部使用的是物理地址。这是裸机下和驱动中操控硬件的一个重要区别。
				(2) uboot早期也是纯物理地址工作的，但是现在uboot开启了MMU做了虚拟地址映射，这个东西驱动也必须考虑。查uboot中的虚拟地址映射表，发现除了0x30000000 - 0x3fffffff 映射到了0xc0000000 - 0xcfffffff之外，其余的虚拟地址空间全是原样映射的。而我们驱动主要是操控硬件寄存器，而S5PV210的SFR都在0xExxxxxxx地址空间，因此驱动中不必考虑虚拟地址。

			3）uboot借用了Linux驱动
				(1) Linux驱动本省做了模块化设计
					Linux驱动本身和Linux内核不是强耦合的，这是Linux驱动易于移植的关键。
				(2) uboot移植了Linux驱动源代码
					uboot是从源代码级别去移植Linux驱动的，这就是Linux系统的开源性。
				(3) uboot中的硬件驱动比Linux简单
					Linux驱动本身有复杂的框架，需要实现更多的附带功能，而uboot本质上只是个裸机程序，uboot移植Linux驱动时只是借用了Linux驱动的一部分而已。

		2、iNand/SD驱动解析
			1）从start_armboot开始
				(1) 驱动整体很庞大，涉及到文件很多，函数很多，布尔西是必须有顺序。
			2）mmc_initialize
				(1) uboot/drivers/mmc/mmc.c（board.c中的start_armboot中的603行）
				(2) 从名字可以看出，这个函数的作用就是初始化开发板上MMC系统。MMC系统的初始化应该包含这么几部分：Soc里面MMC控制器初始化（MMC系统时钟的初始化、SFR初始化）、SOC里面MMC相关GPIO的初始化、SD卡/iNand芯片的初始化。
				(3) mmc_devices链表全局变量，用来记录系统中所有注册的SD/iNand设备。

			3）cpu_mmc_init
				函数位于：uboot/cpu/s5pc11x/cpu.c中。其实质就是调用三个函数完成的。
				(1) setup_hsmmc_clock
					uboot/cpu/s5pc11x/setup_hsmmc.c中。 初始化 高速mmc控制器时钟的。
				(2) setup_hsmmc_cfg_gpio
					uboot/cpu/s5pc11x/setup_hsmmc.c中。配置soc中mmc控制器相关的gpio。
				
				(3) smdk_s3c_hsmmc_init
					boot/drivers/mmc/s3c_hsmmc.c中。这个函数通过宏定义决定调用具体的函数。
					1）s3c_hsmmc_initialize()
						1、 函数位于：uboot/drivers/mmc/s3c_hsmmc.c中。
						2、 定义并且实例化一个 struct mmc *mmc的对象，然后填充这个mmc结构体，最后调用 mmc_register(mmc) 注册进系统。
						3、 mmc_register这个函数就是进行mmc设备注册，注册方法其实就是将当前这个strcut mmc使用链表连接到mmc_devices这个全局变量中去。
						4、 x210中定义了USE_MMC0和USE_MMC2,因此在我们的uboot初始化时会调用2次啥c_hsmmc_initialize函数，传递参数分别是0和2，因此完成之后系统会注册上2个mmc设备，表示当前系统中有2个mmc通道工作。
				(4) find_mmc_device
					uboot/drivers/mmc/mmc.c中。这个函数其实就是通过mmc的设备编号查找对应的对应的mmc设备（mmc_register注册的设备）。
				(5) mmc_init
					找到了相应的mmc后继续初始化mmc卡。
					--> mmc_go_idle
						--> mmc_send_cmd
					--> mmc_send_if_cond
						--> mmc_send_cmd
					--> mmc_send_app_op_cond
						--> mmc_send_cmd

					分析可知：这个函数就是向mmc发送cmd，初始化sd卡内部的控制器。

			总结：mmc初始化分为两部分：一、soc控制器初始化。主要在cpu_mmc_init中完成。二、sd卡初始化。主要在mmc_init中完成。
				 整个初始化完成后去使用sd卡/iNand时，操作方法和mmc_init函数中初始化SD卡的操作一样的方式。读写sd卡时也是通过总线向SD卡发送命令、读取/写入数据来完成的。

		3、 iNand/SD驱动解析
			1）struct mmc
				(1) 驱动设计中有一个关键数据结构。譬如MMC驱动动的结构体就是struct mmc，这些结构体中包含一些成员属性和一些函数指针，变量用来记录驱动相关的一些属性，函数指针用来记录驱动相关的操作方法。这些变量和函数指针加起来就构成了驱动。驱动就被抽象为这个结构体。
				(2) 一个驱动工作主要就分为几部分：
					驱动初始化（构建一个struct mmc然后填充它）
					驱动运行时（调用这些函数指针指向的函数和变量）
			2）分离思想
				(1) 分离思想就是说，在驱动中将操作方法和数据分开。
					数据：变量
					操作方法：函数
					意义：在不同的地方来存储和管理驱动的操作方法和变量，这样的有时就是驱动便于移植。
			3）分层思想
				(1) 一个完成的驱动分为好多个层次。简单理解就是一个驱动分为好多个文件。譬如本课程讲的mmc的驱动涉及到drivers/mmc下面的2个文件和cpu/s5pc11x下的好几个文件。
				(2) 以mmc驱动为例来分析各个文件的作用。
					uboot/drivers/mmc/mmc.c：本文件的主要内容是和MMC卡操作有关的方法，譬如MMC卡设置空闲状态的、卡读写数据等。单数本文件中并没有具体的	硬件操作。操作最终指向的是struct mmc结构体中的函数指针，这些函数指针是在驱动构建的时候和真正硬件操作的函数寡姐的（真正的硬件操作的函数在别的文件中）
					uboot/drivers/mmc/s3c_hsmmc.c：本文件中是soc内部MMC控制器的硬件操作的方法，譬如向SD卡发送（s3c_hsmmc_send_command）命令的函数	，譬如和SD卡读写数据的函数（s3c_hsmmc_set_ios），这些函数就是具体操作硬件的函数也就是mmc.c中需要的那些硬件操作函数。在mmc初始化构件时挂接的。
				分析：mmc.c 和 s3c_hsmmc.c构成了一个分层，mmc.c中调用了s3c_hsmmc.c中的函数，所以mmc.c在上层，s3c_hsmmc.c在下层。这两个分层后我们发现mmc.c中不涉及具体硬件的操作，s3c_hsmmc.c中不涉及驱动工作时的时序操作。因此移植的时候就有好处：
						譬如我们要把这一套mmc驱动移植到别的soc上mmc.c就不用动，s3c_hsmmc.c动就可以了。
						譬如SoC没变但是SD卡升级了，这时候只需要更换mmc.c，不需要更换s3c_hsmmc.c即可。

			4）Linux驱动前奏

	第七部分：uboot移植
		1、 移植前的准备工作
			1）三星移植过的uboot源代码准备
				(1) 三星对于S5PV210的官方开发板为SMDK210,对应的移植过的uboot是：
							F:\朱有鹏播放\朱有鹏\课件and代码\课件\2.uboot和linux内核移植\2.3.零距离初体验uboot/android_uboot_smdkv210.tar.bz2
				(2) 这个源代码网上是下载不到的，三星官方是不会把这些东西放在官方上下载的 。这些东西都会随着官方的开发板一起流出的。

				(3) 移植的时候最重要的工作就是看代码、改代码然后编译运行测试。

				(4) 编译代码必须在Linux中（windows共享文件夹中配置uboot不行的），看代码和改代码可以再Linux中（vim、gedit）也可以在windows中（Sourceinsight）。
				(5)  
			2）移植初体验
				(1) 直接编译三星移植版uboot尝试运行
					1、 赋值到Linux的原生目录下，解压
					2、 检查Makefile中的较差编译工具链
					3、 配置时使用：make smdkv210single_config，对应/include/configs/smdkv210single.h头文件。
					4、 配置完成后直接make编译，便已完成后就进入烧录步骤。
					5、 uboot/sd_fusing目录下有sd_fusing.h脚本，用来烧录。

					运行结果：第一、串口无任何输出
							 第二、开发板供电锁存成功

						分析运行结果：uboot中串口最早的输出在 "OK" ，在lowlevel_init.S中初始化串口时打印出来的；串口无输出 "O"说明在打印 "O" 之前代码已经死掉了；开发板供电锁存在lowlevel_init.S中，开发板供电锁存成功说明这个代码之前的部分是没问题的。结合得出：错误在开发板供电锁存代码和串口初始化之间。

				(2) 建立sourceinsight工程
				(3) 顺藤摸瓜去找可能出问题的地方

				整个程序运行是从start.S开始的。看代码也从这里开始。
				实际上我们只是屏蔽了 	bl PMIC_InitIp 就启动起来了。但是很多信息是有问题的，很多功能应该也是不能用的，都要去一一检查。
		
		2、 时钟和DDR的配置移植
			1）时钟
				(1)更改 #define CONFIG_IDENT_STRING	为 " for Master_r_210" ，make distclean； make smdkv210single_config; make 然后烧录运行。

				(2）时钟部分的运行结果本来就是对的，时钟部分的代码在lowlevel_init.S中的  bl system_clock_init 调用的这个函数中，这个函数的代码部分没任何问题，要改的的是寄存器写入的值，这些纸都在配置头文件（smdkv210single.h）中用宏定义出来了。如果时钟部分要更改，关键是去更改头文件中的宏定义。 （因为芯片是三星的，这里其他厂商是无法更改的，所以这些配置也是正确的，我们需要更改的事board级别的）
				(3) 三星移植时已经把210常用的各种时钟配置全都计算好用宏开关来控制了。只要打开相应的宏就能将系统裴志伟各种不同的频率。

			2）DDR配置信息的更改
				(1) 从运行信息以及bdinfo命令看到的结果，显示DRAM的bank0和1的size值都设置错误了。
				(2) shiyong md和mw测试内存，发现20000000和40000000开头的内存都是可以用的，说明代码中DDR初始化的代码是正确的，只是size错了。
				(3) 内存部分配置完成。

			3）DDR地址另外配置
				(1) 目标：将DDR端口0地址配置为30000000开头
					1、 体验内存配置的更改过程
					2、 改成3开头的地址DRAM bank1和40000000开头的地址就连起来了。这样我们就得到了连续的512MB内存，而原来我们得到的12MB内存地址是断续的

				(2) DDR初始化参数更改
					1、 DDR的初始化代码部分是在lowlevel_init.S中写的，是不动的。代码部分就是对相应的寄存器做相应值的初始化：摇动的是值，而uboot为了具有可移植性吧值都宏定义在include/configs/xxx.h中了。因此我们只需要去这个配置头文件中的配置即可。
				(3) smdkv210single.h中相应宏定义修改
					1、 #define DMC0_MEMCONFIG_0	0x20E0132 改为 #define DMC0_MEMCONFIG_0	0x30E0132 
					2、 寄存器的值改了后相当于是硬件胚子部分做了更改。但是uboot中DDR相关的一些软件配置值还没更改，还在原来位置，需要去更改。
				(4) 虚拟地址映射表中相应修改
					1、 uboot中开启了MMU，堆内存进行了短时映射，有一张内存映射表。之前课程中分析过，分析方法是一样的。经过分析：发现这个内存映射只是把20000000开始的内存映射到c0000000
					2、 为了安全起见，再去配置头文件smdkv210single.h中查看一遍，看看有没有其他的宏定义和内存配置有关的。

					重新配置编译，烧录检查。
				

			4）DDR初始化参数更改2
				(1) 修改DMC0的配置参数
					修改DDR中 #define DMC0_MEMCONFIG_0	0x30E01323 --> #define DMC0_MEMCONFIG_0	0x30F01323 然后sync 编译 烧写 运行。第二部分部分运行。
					分析问题，寻找解决方案。分析方法有2中：1、 第一种靠经验、靠发现能力、靠直觉去找。 2、 就是在整个代码中线基本定位错误地方，然后通过在源代码中添加打印信息来精确定位除出错的代码，然后找到精确的出错位置后再去分析错误原因，从而找到解决方案。

				(2) 修改虚拟地址到物理地址的映射函数
					1、 修改uboot/board/samsung/smdkc110/smdkc110.c中virt_to_phy_smdkc110函数中的 return (addr - 0xc0000000 + 0x20000000); --> return (addr - 0xc0000000 + 0x30000000);
				
				总结：前已发而动全身


		3、 iNand驱动问题解决
			1）先从现象乘除法定位问题
				(1) 定位问题：在源代码中找到导致这个问题的那一句或者几句代码。有时候需要修改的代码和hi姐导致这个问题的代码是不同的。
				(2) 定位问题后，解决问题并不一定要改问题出的代码。但肯定与这又代码有关，通过分析这其中的关联。
				(3) 搜索定位到 818 行。 （搜索关键字）

			2）网络搜索解决方案 
				(1) 首先看一下问题出上下文。
				(2) 通过代码上下文，发现这个函数在读取SD/iNand的ext_csd寄存器的值。通过浏览代码结合出错地方，可以判断出：从卡断读取ext_csd寄存器是成功的，并且从读取结果中拿到了卡的版本好信息。然后代码对版本号进行了判断，并且如果版本号大于5就会报错退出。
			3）尝试修改代码解决问题
				(1) 改判断依据

			4）推测和实验验证（SD卡和iNand的区别）
				(1) 板子上有一个iNand接在SD0上，有一个外置SD卡接在SD2上。那uboot中初始化的这个是iNand而不是SD卡。也就是说uboot中实际用的是SD0。
				(2) 使用SD卡则不会出现上面的问题，因为SD卡版本比较低。

			5）小问题的修补
				(1) 控制台串口改为串口0
					1、 #define CONFIG_SERIAL3 --> #define CONFIG_SERIAL1
							--> #define ELFIN_UART_CONSOLE_BASE
							lowlevel_init中汇编初始化串口

				(2) 修改默认网络地址设置
					1、 在配置文件中修改 smdkv210single_config 中修改相应宏即可。 注意：修改的是默认环境变量（uboot默认使用iNand中的env）
					2、 mmc write 0 3000000 11# 33
					表示将DDR的0x30000000 开头的一段内存中的内容写入iNand中的第17个扇区开始的32个扇区内，写入长度是32个扇区长度（16KB）
				(3) 修改行提示符
					1、 修改配置文件相应的宏。
				(4) 总结
					1、 基于soc厂商的uboot我们主要更改我们使用的配置文件，我们只需要修改bsp级别的配置，uboot源码基本不用修改。
					2、 对uboot的结构了解了，uboot移植就非常简单。

		4、 网卡驱动移植1
			1） 网卡芯片与开发板的链接方式
				1、 SoC的SROM bank和网卡芯片的CS引脚 （SROM就是SRAM/ROM）。SoC的SROMController其实就是SoC提供的对外总线式链接SRAM/ROM的接口。如果要外部外接一些SRAM/ROM类的存储芯片（或者伪装成SROM接口的芯片，譬如网卡）就要通过SROMController来链接。
				网卡接在SROM中的好处就是：网卡芯片像一个存储器被扩展主机SoC的一个地址空间中，主机SoC可以直接用一个地址来访问网卡芯片内部寄存器。
				2、 网卡芯片内部寄存器使用相对地址访问
					网卡内部寄存器有一个地址，这个地址是从00开始的，但是实际上我们SoC不能用0地址去访问这个网卡的寄存器，需要使用相应的SROM的基地址去访问。
					主机上网，其实就是通过操控网卡芯片内部的寄存器、缓存区等资源来上网的。也就是说其实SoC是通过网卡芯片来简介上网的。
				3、 总结：实际上也是一种总线式链接方式。优势是SOC内部不需要内置网卡控制器，所有的SFR全都在外部网卡芯片中，而且可以通过地址直接访问（IO与内存同意编址），不想Nand/SD接口一样使用时序来访问。

				4、 从逻辑上网卡更像是串口，而不像存储器。
			2） 原理图浏览
				1、 210的SROM控制器允许8/16bit的接口，我们实际使用的是16位接口。
				2、 网线里面有8根线，但是实际只有4跟有效通信线，另外4根都是GND，用来抗干扰的。2个TX  2个RX
				3、 网卡芯片有个CS引脚 （chip select），此处接SROM的片选引脚。
				4、 DM9000的CMD引脚接到了S5PV210的ADDR2引脚上。DM9000为了节省资源，数据线与地址线复用，有CMD引脚控制。（CMD = 1，为数据；；CMD = 0 ，为CMD）

			3） 网卡驱动文件介绍
			4）网卡移植的关键

		5、 网卡驱动移植2 
			1） 网卡驱动文件介绍
				(1) uboot中本来就提供了很多网卡芯片的驱动程序，在uboot/drivers/net/dm9000x.c和dm9000x.h。这个驱动来自于Linux kernel源代码。uboot中是移植而不是编写。
				(2) 要向彻底看懂这个驱动，必须对Linux的驱动模型中网络设备驱动有一定的理解才可以。

			2）网卡移植的关键：初始化
				(1) uboot在第二阶段init_sequences中进行了一些列的初始化，其中就有网卡芯片的初始化。这个初始化就是关键，在这里的初始化中只要将网卡芯片正确的初始化了，则网卡芯片就能工作（意思是网卡驱动dm9000x.c和dm9000x.h依赖于这里的初始化而工作。）。
				(1) 网卡初始化代码地方在：
					-->start_armboot 
						-->init_sequence 
							-->board_init 
								-->dm9000_pre_init		这个函数就是移植的关键

				(3) dm9000_pre_init函数主要功能就是初始化DM9000网卡。这个初始化过程和我们开发板上DM9000网卡芯片的硬件连接方式有关。必须要结合开发板原理图来分析，然后决定这个函数怎么编程。

				(4) 原来的代码是三星的工程师根据三星的开发板SMDKV210的硬件接法来写的程序，我们要根据自己的开发板的硬件接法去修改这个程序，让网卡在我们的开发板上工作。

				(5) #define DM9000_16BIT_DATA 这个宏用来表示开发板时工作在16位模式下。

				(6) 从三星版本的代码中可以看出，他操作的事bit20 -- bit23, dm9000 连接到bank5上的。而我们世界在bank1上的，即bit4 -- bit7。

				总结：三个寄存器的修改。主要是三星的开发板DM9000接在bank5，我们接在了bank1上，因此要做一些修改。

			3）基地址的配置等
				(1) 驱动分为两部分：代码和数据。代码不用动，数据要修改。
				(2) CONFIG_DM9000_BASE是DM9000网卡通过SROM bank映射到SoC中地址空间中的地址。这个地址的值取决于硬件接到了那个bank，这个bank的基地址是SoC自己定义好了的。譬如我们这里接到了bank1上，bank1的基地址是 0x88000000 。

				(3) DM9000_IO 表示访问芯片IO的基地址，直接就是CONFIG_DM9000_BASE；DM9000_DATA表示我们访问数据时的基地址，因为DM9000芯片的CMD引脚接到了ADDR2，因此这里要加 +4(0b100,对应ADDR2)。
				(4) 本来这样配置就完了，重新编译运行网卡就应该工作了。但是实际测试发现不工作，要怎么修改呢？修改方式是将 CONFIG_DM9000_BASE改成 0x88000300 就工作了。

				问题：这个0sx300从哪里来的呢？
					最靠谱的解释：跟DM9000的版本有关，内部寄存器就有一个0x300的偏移量。

			4）Linux系统中网卡驱动的典型工作方式简介
				(1) 在Linux系统中，网卡算是一个设备，这个设备驱动工作后会生成一个设备名叫eth（n：1、2、3...）（无线网卡名字一般叫 wlan0、wlan1 ...）。然后Linux系统用一些专用命令来操作网卡，譬如ifconfig命令。
				(2) Linux下的应用程序如何使用网卡驱动来进行网络通信？最通用的方法就是socket接口。Linux系统中有一系列的API和库函数，提供了一个socket编程接口，Linux下的应用程序都是通过socket来实现上网的，socket内部就是间接调用的网卡驱动实现网络通信的。
				(3) Linux设计时非常完备的，应用层和驱动层是严格分离的。也就是说些网络编程的应用层的人根本不用管驱动，只要会用socket接口即可；写底层驱动的人根本不用管应用层，只要会Linux的网络驱动框架模型即可。

			5）uboot中网卡驱动的工作方式简介
				(1) 一定要记住：uboot本身是一个逻辑程序，是一个整体，没有分层。所以uboot中根本没有驱动和应用的概念。
				(2) 按照逻辑来说，ping命令实现的代码及时网络应用程序，像dm9000x.c和dm9000x.h这样的代码属于驱动程序。所以在uboot中这些东西揉在一起的，应用是直接调用驱动实现的。也就是说平命令内部是直接调用了dm9000的网卡驱动中的函数来实现自己的。

			6）以ping命令为例查找代码验证分析
				(1) do_ping
					--> NetLoop
						--> PingStart
							--> PingSend
								--> ArpRequest
									--> eth_send
					--> eth_rx
				(2) 验证了uboot中应用程序（ping）调用驱动程序（dm9000x.c）的方式。

			7）使用自己移植的uboot启动内核
				(1) 问题：当前uboot不能启动内核
					九鼎移植的可以启动内核，我们的不行。我们的uboot有问题。
					做基本检查：1、怀疑机器码不对。但是我们的和九鼎的一样。
							   2、怀疑我们修改了串口导致看不到启动信息。

				(2) 解决问题第一步：将串口改回串口2
					在smdkv210single_config.h中修改相应的宏。

				(3) 根据现象分析，定位问题并试图解决
					1、 

		6、 基于uboot官方uboot源码移植
			1） 找到合适的uboot
				(1) 主要寻找相同CPU的demo，以其为基础移植uboot。

				(2) 找到的对应的cpu、board文件夹分别是
					cpu 	: u-boot-2013.10\arch\arm\cpu\armv7
					board 	: u-boot-2013\board\samsung\goni

			2） mkconfig脚本分析
				(1) 脚本功能浏览
					1、 首先我们在命令行配置uboot时，是：make S5P_goni_config，对应Makefile中的一个目标
					2、 新版本的Makefile中：
						%_config::	unconfig
							#$(MKCONFIG) -A $(@:_config=)
						从这里分析得出：实际配置时是调用mkconfig脚本，然后传参2个：-A 和 S5P_goni_config
						然后到了mkconfig脚本中

					3、 mkconfig脚本中
						使用正则表达式匹配 $1 所在的行截取出来赋值给line，然后将line的内容以空格为间隔依次分开，分别赋值给$1 ... $8。
						此处过后 $1 和 $2 与传进来的值不一样了。
						$1 = Active
						$2 = arm
						$3 = armv7
						$4 = s5pc1xx
						$5 = samsung
						$6 = goni
						$7 = s5p_goni
						$8 = -


					4、 几个传参和其意义
						arch=arm
						cpu=armv7
						vendor=samsung
						soc=s5pc1xx

					5、 符号链接
						include/asm --> arch/arm/include/asm
						include/asm/arch --> include/asm/arch-s5pc1xx
						include/asm/proc --> include/asm/proc-armv

						创建include/config.h文件。 

					6、 解决官方版本uboot的烧录问题
						1）如何烧录
							(1) 烧录u-boot.bin到SD卡有两种方法：windows下用烧录软件；Linux下用dd命令烧录脚本烧录。因为windows下的工具不开源，出了问题没法调试，所以不用。我们用Linux下的烧录脚本烧录（实质就是dd命令进行sd卡扇区写入）。
							(2) 移植原来uboot中的文件夹到官方版本的uboot，来烧录u-boot.bin。

						2）分析为什么烧录运行不了
							(1) 串口2有输出，但是这个输出不是uboot的输出的，而是内部的iROM中的BL0运行时输出的。
							(2) 老朱的sd卡校验和出错，但我的sd卡校验和没错。
						3）解决方案分析
							(1) 老朱SD卡烧录会出错？可能原因：烧录方法出错、烧录原材料出错。
							(2) 经分析老朱的 sd_fusing这个文件下的mkbl1这个程序没问题，上一层的u-boot.bin是存在的，校验和失败和u-boot.bin无关。
							(3) 经分析得知，start.S和mkbl1程序前16字节校验和的处理不同。

							(4) 老朱找到问题的方法。发现校验和不通过，就把可以通过的和不能通过的用相应软件打开查看，发现最开始一行错开了。
						4）代码实践


					7、 start.S分析与移植
						1）start.S流程分析
							(1) #define CONFIG_SYS_TEXT_BASE		0x34800000 
								uboot连接地址是在0x34800000位置。

							(2) save_boot_params 是个空函数，返回0。

							(3) cpu_init_cp15 初始化cp15协处理器。
								这个函数是设置MMU、cache等。

							(4) cpu_init_crit --> lowlevel_init 
								这个 lowlevel_init 是board/samsung/goni/lowlevel_init.c
								1、 经过浏览 lowlevel_init 这个函数做的有意义的事包括 关看门狗、调用uart_asm_init来初始化串口、没有做时钟初始化（下面优势种初始化的函数，但是实际没有调用。如果uboot没有初始化则时钟使用的事BL0阶段初始化的时钟系统）

							(5) 添加开发板供电锁存和串口打印字符 "O"
								1、 我们为了调试uboot第一阶段，就要看到现象。所以我们想lowlevel_init函数中添加2个代码，一个是开发板制锁和串口打印"O"。

							(6) 实践
								1、 失败了，什么结果都没有

								结论：因为开发板制锁没有成功，所以判定，在开发板制锁之前就失败了。继续分析。

							(7) 添加LED+延时代码发现 在 cpu_init_crit 函数前有现象，在其后没有现象。
								进入 cpu_init_crit 函数发现在lowlevel_init前有现象在该函数内一点现象都没有。

								结论： 
									b	lowlevel_init		@ go setup pll,mux,memory 
									lowlevel_init 函数超出了限制，三星规定BL1大小为8KB，所以uboot第一阶段的代码必须在整个uboot镜像的前8KB内，否则跳转不到。

									因此需要将 lowlevel_init 添加到前8KB内，即修改 u-boot.lds 的连接脚本。

							(8) 经分析修改后，编译出错 lowlevel_init 重复定义。
								分析原因得：因为lowlevel_init这个函数被连接了2次。一次是board/samsung/goni这个目录下生成了libgoni.o时连接了一次，第二次是连接脚本最终在连接生成u-boot时有链接了一次。

								错误解决：在libgoni.o中不要连接lowlevel_init。即可避免重复定义。

							(9) 解决办法
								抄  参考当前版本的 start.S的技巧。

						2、 添加DDR初始化1
							1）分析下一步移植路线
								(1) cpu_init_crit函数成功初始化串口、时钟后，转入 _main 函数，函数在arch/arm/lib/crt0.S文件中。
								(2) 在crt0.S中首先设置栈，将sp指向DDR中的地址。然后调用了 board_init_f 函数进行板级初始化。函数在arch/arm/lib/board.c中。
								(3) 在这个版本的uboot中，把以前uboot的第二阶段start_armboot函数分成了两部分：boot_init_f和boot_init_r。所以在这里就和以前版本的uboot接轨上了，推测boot_init_f中肯定做了板级初始化，board_init_r中进入了uboot的命令行。
								(4) 分析到这里，在uboot2013.10版本中思路已经很清晰了：uboot的第二阶段就在crt0.S文件正，第二阶段的入口就是 _main 函数。第一阶段主要就是cpu_init_crit函数，所以我们要在cpu_init_crit函数中添加DDR初始化和uboot的重定位。
								(5) 分析到这里，下一步工作方向就确定了。我们要现在cpu_init_crit函数中添加DDR初始化，然后在start.S 中bl _main之前添加uboot的重定位，然后将bl _main gaicheng ldr pc, __main 。 （__main: .word _main）长跳转。然后在crt0.S中board_init_f后删除那些重定位代码，至此uboot的第二阶段就应该启动起来了。后续的移植就是第二阶段了。

							2）分析DDR初始化代码移植思路
								(1) 如果uboot中有DDR初始化代码，那我们可以就这这些代码来修改。但是uboot-2013.10中没有DDR初始化代码，所以我们需要完全从头添加DDR初始化代码。
								(2) 我们的方法就是 直接从三星版本的uboot移植DDR代码。三星版本的uboot中DDR初始化代码在cpu/s5pc11x/s5pc110/cpu_init.S文件中，直接将这个文件移植过来即可。

							3）手动添加
								(1) 添加cpu_init.S文件到uboot-2013.10中。注意：这里的代码必须保证在前8KB内，所以必须和lowlevel_init.S文件一样连接处理。 主要是在board/samsung/goni/Makefile中和arch/arm/cpu/u-boot.lds中添加。

								(2) 添加头文件 s5pc110.h到include目录下
								(3) 对cpu_init.S文件代码尽心修整，把一些无用的代码去掉，吧一些相关的条件编译人工处理下。

							4）移植DDR驱动2
								(1) 移植必要的宏定义
									DDR配置参数，从三星版本的smdkv210single.h中复制到s5p_goni.h中。

								(2) s5pc110.h 中进行修整。

								(3) sync 编译 再修改。


						3、 添加uboot第二阶段重定位
							1）uboot重定位1
								(1) 在重定位代码前加调试信息定位
									1、 逻辑上来说，重定位部分代码应该在DDR初始化之后，且在uboot跳转到第二部分之前。

									2、 uboot的第一阶段和第二阶段的划分并不是绝对的，位移必须遵守的原则就是第一阶段不能大于8KB。所以uboot第一阶段最潮要完成DDR初始化，以及第二阶段重定位，最多不能超过8KB。在满足这些条件时，第一阶段和第二阶段节点可以随便挑。

									3、 找到合适的地方来写重定位代码，重定位之后跳转到第二阶段的入口。

								(2) 重定位代码
								(3) 清bss段

								(4) 在 _main 函数中删除关于重定位的代码后剩下 board_init_f 和 board_init_r这两个主要看书。

								(5) 解决编译问题
									1、 movi.h 中宏定义出错，在s5p_goni.h中添加 CONFIG_EVT1 宏。

									2、 连接错误：u-boot contains relocations other than 	R_ARM_RELATIVE

										在u-boot源码目录下直接搜索，R_ARM_RELATIVE ： grep "R_ARM_RELATIVE" -nR *
										发现在主Makefile中有链接规则的检测存在导致这个问题，屏蔽规则里面的内容即可。


						4、 时钟配置 
							1）

						5、 board和DDR配置显示移植
							1）关于MACH_TYPE的问题
								以前是在自己的配置文件中定义的，而在2013-10版本中几种定义在一个文件arch/arm/include/asm/mach-types.h。

								几种定义是从Linux内核中学来的。在Linux kernel中MACH_TYPE就是在文件中几种定义的。方便查阅，不容易重复。这个MACH_TYPE是和开发板绑定的。 是由Linux kernel管理者分配的，不能随便用。


						6、 board_init_r移植
							1）去掉oneNand支持
							2）天剑SD/MMC支持

						7、 uboot-2013.10中SD/MMC驱动浏览
							1）从初始化代码开始浏览
							2）相关函数和文件
								drivers/mmc/mmc.c
								board/samsung/goni/goni.c
								arch/arm/include/asm/mmc.h
								drivers/mmc/sdhci.c
							3）当前错误定位及解决方案分析
								board.c
									--> board_init_r
										--> mmc_initialize
											-->do_preinit
												--> mmc_start_init
													--> mmc_go_idle
														--> mmc_send_cmd
															--> mmc->send_cmd == sdhci_send_command
																--> sdhci_transfer_data
																	--> printf("Error detected in status(0x%X)!\n", stat);
							4）错误原因分析
								sdhic.c中的所有函数构成了三星210CPU的SD/MMC控制器的驱动。这里面的函数是三星公司的工程师写的，内容就是用来控制210CPU的内部的SD/MMC控制器和外部的SD卡通信的。
								sdhci_transfer_data函数出错，说明是SoC的SD/MMC控制器和外部SD卡（其实现在用的事SD0的iNnad）的数据传输出了问题。（细节分析发现是控制器内部有一个中断状态错误标志被置位了）

							5）解决方案分析

								两条思路：
									1、 逐行分析SD卡驱动实现（分析中要对SD卡通信协议和210这个SoC的SD控制器非常熟悉），然后发现错误所在，然后修改代码解决问题。
									2、 投机取巧，超三星移植版本的uboot中的SD/MMC驱动整个移植过来替换uboot-2013.10中的MMC驱动。
									3、 综合 1、2 参考三星版本的uboot中的驱动实现来修补uboot-2013.10中的驱动实现。

							6）SD卡驱动移植
								1、 分析两个版本uboot中SD卡驱动差异
									uboot2013.10  相关文件有： 
										drivers/mmc/mmc.c
										drivers/mmc/sdhci.h
										drivers/mmc/s5pc110.c
										board/mmc/s5p_goni.c 

									samsung版本
										drivers/mmc/mmc.c
										drivers/mmc/s3c_hsmmc.c
										cpu/s5pc11x/cpu.c
										cpu/s5pc11x/setup_hsmmc.c

									经过分析发现：SD卡驱动要工作包含2部分内容，一部分是drivers/mmc目录下的事驱动，另一部分是uboot自己提供的初始化代码（譬如：GPIO初始化、时钟初始化）
								2、 赋值必要的文件并修改相应的Makefile
									首先解决drivers/mmc替换
									修改初始化代码
								3、 代码浏览及修补
									(1) 按照代码运行时的流程程来逐步浏览代码，看哪里需要修补。
								
								4、 同步及编译、问题解决
								5、 效果测试



						8、 环境变量
							1）iNand分区表检车 -env 究竟应该放在那里
								(1) 测试环境变量是否可以保存
									结果：ok

								(2) 我们的环境变量究竟放到哪里去了？
									分析代码中的分区表。

								(3) 应该放在那里？ 无法确定，但是有些地方绝对不能放。原则上同一个扇区只能放一种东西，不能叠加，否则就会被覆盖掉。uboot烧录时使用的扇区数是：
									SD2的扇区 1-16 49 -x (x-49 大于等于uboot的大小)
								(4) 从uboot的烧录情况来看，SD2的扇区空闲，扇区1-16被uboot的BL1占用，扇区17-49空闲，扇区49-x被uboot的BL2占用。再往后就是内核、rootfs等镜像的分区了。系统一直工程师可以分局kernel镜像大小、rootfs大小等自由给SD分区。
								(5) 

							1）环境变量相关代码浏览
								1）目前情况是uboot在SD2中，而ENV在SD0中，所以现在ENV不管放在哪个扇区都能工作，不会有问题。但是我们还是得找到ENV分区所在并且改到不会和uboot冲突，因为将来部署系统时我们会将uboot、kernel、rootfs等都烧录到iNand中去，那时候也要确保不会冲突。
								2）static inline int write_env(struct mmc *mmc, unsigned long size,
			    						unsigned long offset, const void *buffer)
									类似这种函数，在代码中关键是弄明白参数的意义。
									mmc：mmc设备
									size：大小
									offset：SD卡的那个扇区
									buffer：要写的内容


									ALIGN(offset, mmc->write_bl_len) 扇区对其

								3）CONFIG_ENV_OFFSET这个宏决定了我们的ENV在SD卡中相对SD卡扇区0的偏移量，也就是ENV写到SD卡的哪里去了。经过分析发现这个宏的值为0，所以我们的ENV被写到了0扇区开始的32个扇区中。
								4）这列回合uboot的BL1冲突，必须改。
								5）#define MOVI_BL2_POS		((eFUSE_SIZE / MOVI_BLKSIZE) + MOVI_BL1_BLKCNT + MOVI_ENV_BLKCNT)  后面这三个分别是 1 + 16 + 32

								1：扇区0
								16：扇区1-16 存放uboot的BL1
								32:17-48 存放env的

								6）我们参考即可，只要把env放在17扇区开始的地方就可以

							2）环境变量的测试和配置移植
								(1) 擦除前49个扇区，确保前49个扇区是空的。
									mmc write 0 30000000 0# 49 使用这个命令擦除 0 - 48

								(2) reset后set 然后save
								(3) 测试方法：mmc read 0 30000000 17# 32 命令将iNand的第17个扇区开始的32个扇区读到 内存的30000000处


						9、 网络移植
							1）添加网络支持宏，屏蔽： #undef CONFIG_CMD_NET 即可。

							2）添加ping和tftp命令。
								(1) 在Linux系统中网络底层驱动被上层调用的接口是socket，是一个典型的分层结构，底层和上层是完全被socket接口隔离的。

								(2) 在uboot中网络底层驱动和上层应用是黏在一起的，不分层。意思就是上层网络的每一个应用都是自己去调用底层驱动中的操作硬件的代码来实现。

								(3) uboot中很多预先设计的需要用到网络的命令，和我们直接相关的就是ping和tftp这两个命令。这两个命令在uboot中也是需要用相应的宏开关来打开和关闭

								(4) 经过代码检查，发现ping命令开关宏为 CONFIG_CMD_PING, tftp相关的宏为 CONFIG_CMD_NET

							3）代码实践
								(1) 都有了，但是都不能用。硬件为初始化成功。

							4）网卡实践现象分析：
								我们没有自定义的网卡出事啊函数 (board_eth_init 和 cpu_eth_init)，所以uboot启动网卡是打印了： Net:   Net Initialization Skipped

								(2) eth.c中有2个很重要的全局变量： eth_device (用来指向一个链表，这个链表中保存了当前系统中所有的网卡信息) 和 eth_current。
								(3) 在Linux的网卡驱动体系中，有一个数据结构（struct eth_device）用来表示（封装）一个网卡的所有信息，系统中注册一个网卡时就是要建立一个这样的结构体的实例，然后填充这个实例中的元素，最后将这个结构体实例加入到eth_devices这个链表上，就完成了注册。了解了这些后，你就明白了网卡驱动在初始化时必须负责将自己注册到系统的网卡驱动体系中（其实就是把自己的eth_device结构体实例添加到eth_devices链表中）。如果你不做这个过程就会出现：网卡找不到的错误。

								(4) 注册网卡函数在Dm9000.c中的 dm9000_initialize(bd_t *bis)。









Linux内核文件分析
	1、单个文件
		1）Kbuild kernel build。这个文件就是Linux内核特有的内核变异体系需要用到的文件。

		2）Makefile Linux内核的总Makefile，整个内核工程用这个Makefile来管理的。

		3）mk，是九鼎添加的，不是Linux内核本身的东西。目的是为了整体配置编译kernel目录，

	2、目录
		1）arch 架构

		2）block 块设备，存储设备。此目录下存放关于块设备的代码。

		3）crypto 加密相关。

		4）drivers 驱动目录，里面列出来了kernel支持的所有硬件驱动源码。

		5）firmware 固件。

		6）fs file system 文件系统。

		7）include 头文件目录，公共的（各种CPU架构共用的）头文件。特有的在arch/arm/include目录下。

		8）init linux内核启动时初始化内核的代码。

		9）ipc inter process commuication 进程间通信。Linux中支持的IPC的代码实现。

		10）kernel Linux内核。内核本身需要的代码文件。

		11）lib。 共用的库函数，注意这里的库函数和C语言的库函数不一样的。在内核编程中是不能用C语言标准库函数，这里的lib目录下的库函数就是用来替代那些标准库函数的。

		12）mm memory management，内存管理，Linux的内存管理代码都在这里。

		13）net 网络相关的代码。TCP/IP 

		14）scripts 脚本。不是Linux内核工作使用的，而是用来辅助Linux编译配置的。

		15）security 安全相关的代码。

		16）sound 音频处理相关的。

		17）tools Linux中一些有用的工具。

		18）user  initfamfs相关的，与Linux内核的启动有关，暂时不用去管。

		19）virt kernel virtual machine。内核虚拟机


		与我们关系紧密的就是arch 、 drivers目录，除此之外还有 include 、 block 、 mm 、 net 、 lib等。


	3、内核配置和编译
		1）先确认Makefile
			1、 主要是检车较差编译工具链是否设置对。
				CROSS_COMPILE   ?= /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-
				ARCH  = arm

		2）make x210ii_qt_defconfig
			最后出现： configuration written to .config  说明配置成功，没有出现则出错。

			可能出现的错误
			错误原因： 配置项拼写出错

		3）make menuconfig
			结果：出现图形化界面

			可能出现的错误
			错误原因： 配置项拼写出错
					  ncurses库没装（解决办法网上搜）
					  屏幕太小

		4）make 
			结果： 不在当前目录下，在arch/arm/boot目录下 zImage


	4、 内核配置原理1
		1）配置的关进是得到 .config 文件
			1、 .config以.开头，是一个隐藏文件，需要ls -a来看。
			2、 .config文件时Linux内核在编译过程中很重要的一个文件，起作用类似于uboot中的include/configs/x210_sd.h ，内核在编译过程中会读取 .config 中的配置项，并且这些配置想去指导真个编译链接过程。
			3、.config 文件的格式类似于脚本文件，其中内容为类似于：CONFIG_ARM=y 的一个一个配置项，即每一行一个配置项。

			4、 make xx_defconfig 和 make menuconfig相配合

		2）第一步： make xx_defconfig   参考xx配置，减少工作量，避免一些自己不懂的（例如内存管理、调度系统）
		   第二部： make menuconfig 	   读取第一步的结果，以图形化界面显示，

		  3）make xx_defconfig到底做了什么？
		  	cp arch/arm/configs/x210ii_qt_defconfig .config

		  	make savedeconfig  保存修改后的.config

	5、内核配置
		menuconfig使用说明
			1）make menuconfig中本身自带的提示就有所有的用法，这里只要全部理解就可以了。
			2）menuconfig 中间的选择区中有很多个选择项，每个选择项对应.config文件中的一个配置项，每一个选择项都可以被选择和配置操作，选择去中的每一项都是有子目录的。

		menuconfig工作原理
			1）menuconfig本身有一套软件支持
				1、Linux为了实现图形化界面的配置，专门提供了一套配置工具menuconfig。
				2、ncurses库是Linux中用来实现文字式的图形化界面，Linux内核中使用了ncurses库来提供menuconfig。
				3、scripts/kconfig/lxdialog目录下的一些c文件及时用来提供menuconfig的那些程序源代码。

			2）menuconfig 读取 Kconfig 文件信息
				1、 menuconfig本身的软件只负责提供menuconfig工作的这一套逻辑（譬如在menuconfig中通过上下左右箭头按键来调整光标，enter选择，esc返回等），而不负责提供内容（菜单里的项目）
				2、menuconfig显示的菜单内容（一方面是菜单的目录结构，另一方面是每一个菜单项目的细节）是由内核源码树各个目录下的 Kconfig 文件来支持的。Kconfig文件中按照一定的格式包含了一个又一个的配置项，每一个配置项在make menuconfig中都会成为一个菜单项目。而且menuconfig中显示的菜单目录结构和源码目录中的Kconfig的目录结构是一样的。

			3）menuconfig 和 .config的关系
				1）menuconfig的菜单内容来自于Kconfig文件，但是每一个菜单的选择结果（Y、N、M）却不是保存在Kconfig中，而是保存在.config中。 
				2）make menuconfig在打开时，会读取.config 文件，并且有那个.congig 文件中的配置选择结果来初始化menuconfig中各个菜单项的选择值。

				总结：菜单项的项目内容从Kconfig文件来，菜单项的选择值从.config文件来。

		Kconfig文件详解
			1）Kconfig的格式
				1、 Kconfig按照一定的格式来书写，menuconfig程序可以识别这种格式，然后从中提取有效信息组成menuconfig中的菜单项。
				2、 Kconfig文件语法
					menuconfig xxx  （本身也是一个菜单项）表示这是有一个子菜单项 xxx
					config 			表示xxx菜单中的一个选项

				3、 menuconfig 和 config后空格后的 NETDEVICES 与CONFIG_ 结合就组成了 .config中的配置项。 

				4、	一个menuconfig后面跟着的config就是这个 menuconfig的子菜单。这就是Kconfig中表示的目录关系。

				5、	内核源码目录书中每一个Kconfig都会source引入其所有子目录下的Kconfig，从而保证了所有的Kconfig项目都被包含进menuconfig中。

			2）tristate 和 bool的含义
				tristate：三态 Y N M
				bool :	Y N 

			3）depends的含义
				1、 依赖 
				2、 本配置项依赖于另一个配置项，如果另一个被配置项为Y M则本配置项才有意义，
				3、 depends并不要求以来的配置项一定是一个，可以是多个。

			4）help 
				1、 帮助信息，告诉我们怎么配置。

			5）select 
				1、 自动选择

			6）default
				1、 默认选择值

	6、kernel分析
		主Makefile
			版本 架构 交叉编译工具链

		连接脚本 
			kernel不直接提供连接脚本，而是提供一个汇编文件vmlinux.lds.S（位于arch/arm/kernel目录下），然后在编译的时候再去编译这个汇编文件得到真正的连接脚本vmlinux.lds。
			思考：为什么不直接提供连接脚本？
				猜测：.lds只能写死，不能用条件编译。但是我们在kernel中连接脚本确实有条件编译的需求（但是lds格式又不支持）。 

		kernel启动入口。
			从vmlinux.lds.S中可以入口为stext，在si中搜索知 head.S （启用了MMU情况下的kernel启动文件，相当于start.S）。head-nommu.S是未使用mmu情况下的kernel启动文件。

		Head.S文件分析
			1）内核运行的物理地址与虚拟地址
				(1) 
					#define KERNEL_RAM_VADDR	(PAGE_OFFSET + TEXT_OFFSET)   内核运行的虚拟地址 0xC0008000
					#define KERNEL_RAM_PADDR	(PHYS_OFFSET + TEXT_OFFSET)	  内核运行的物理地址 0x30008000

				(2) 内核的真正入口
					CONFIG_XIP_KERNEL  本地执行(内核在哪就在那执行)

					真正入口： ENTRY(stext)
					前面的 __HEAD 定义了后面的代码是 .head.text

				(3) 内核运行的硬件条件
					1）内核的起始代码是被解压代码调用的。会议之前讲的zImage的时候。uboot启动内核后实际调用运行的事zImage前面的那段未经压缩的解压代码，解压代码运行时先将zImage后段的内核解压开，然后调用运行真正的内核入口。
				
					2）内核启动不是无条件的，而是有一定的先决条件，这个条件又启动内核的bootloader（我们这里就是uboot）来构建保证。
				
					3）ARM体系中，函数调用时实际是通过寄存器传参的（函数调用时传参有两种设计：一种是寄存器传参，另一种是栈内存传参）。所以uboot中最后 thekernel(0,machid,bd->bi_boot_params);
						执行内核时，运行时实际把0放入r0中，machid放入到了r1中，bd->bi_boot_params放入到了r2中。ARM的这种处理技巧刚好满足了kernel启动的条件和要求。
				
					4）kernel启动时MMU是关闭的，因此硬件上需要的是物理地址。但是内核时一个整体（zImage）只能被连接到一个地址（不能分散加载），这个链接地址肯定是虚拟地址。因此内核运行时前段head.S中尚未开启MMU之前的这段代码就很难受。所以这段代码必须是位置无关代码，而且其中涉及到操作硬件寄存器等时必须使用物理地址。

			一、内核的汇编阶段
				1、 __lookup_processor_type 
					1、 我们从cp15协处理器的c0寄存器中读取硬件的CPU ID号，然后调用此函数来进行合法性检验。合法就继续启动，不合法则停止启动，转向 __error_p 启动失败。
					2、 该函数检验cpu id的合法性方法是：内核会维护一个本内核支持的CPU ID号码数组，然后该函数所做的就是将硬件这种读取的cpu id号码和数组中存储的各个id号码一次对比，如果没有一个相等则不合法，如果有一个相等的则合法。
					3、 内核启动时设计这个检验，也是为了内核启动的安全性着想。

				2、 __lookup_machine_type
					1、 该函数的设计理念和思路和上面校验cpu id的函数一样的。不同之处是本函数检验的是机器码。

				3、 __vet_atags
					1、 设计思路同上，又来校验uboot传参ATAGS格式是否正确。这里说的传参指的是uboot通过tag给内核传的参数（主要是板子的内存分布memtag、uboot的bootargs）
					2、 内核认为如果uboot给我的传参格式不正确，那么我就不启动。
					3、 uboot传参不对会导致，时会导致内核不启动的。譬如uboot的bootargs设置不正确内核可能就会不启动。

				4、 __create_page_tables
					1、 建立页表。
					2、 Linux内核本身是被连接在虚拟地址处，因此kernel希望尽快建立页表并且启动MMU进入虚拟地址工作状态。但是kernel本身工作起来后页表体系是非常复杂的，建立起来也不是那么容易的。kernel想了一个好办法。
					3、 kernel建立页表其实分为2不。第一步，kernel先建立了一个段氏页表（和uboot中之前建立的页表一样，页表以1MB为单位来区分的），这里的函数就是建立短时页表的。段氏页表本省比较好建立（段氏页表1MB一个映射，4GB的空间就需要4096个也表项，每个也表项4字节，因此一共需要16KB内存来做页表），坏处是比较粗不能精细管理内存；第二部，再去建立一个细页表（4kb为单位的细页表），然后启用新的细页表废除第一步建立的段氏映射表。
					4、 内核启动的早期建立段氏页表，并在内核启动前期使用；内核启动后期就会再次建立细页表并启用，内核巩固走起来后就只有细页表了。

				5、 __switch_data
					1、 建立了页表后（段氏页表）接入了 __switch_data 部分，这个东西是函数指针数组
					2、 下一步要执行 __mmap_switched
						作用： Copy data segment 、 Clear BSS  构建C语言运行环境
						保存 cpu id号，机器码，tag传参首地址。
						b	start_kernel 跳转到C语言运行阶段。

			总结： 校验（cpu id、机器码、传参） --> 建立粗页表开MMU --> 准备C语言环境（copy data segment、 Clear BSS） --> 跳转。

			二、C语言阶段
				1、 1）学习思路
						1、 抓大放小，不深究 
						2、 感兴趣可以就某个话题去网上搜索资料学习。
						3、 重点局部深入分析。

					2）具体学习方法
						1、 顺着代码执行路径抓全局
						2、 对照内核启动的打印信息进行分析

					3） 几条学习路线
						1、 分析uboot给kernel传参的影响和实现
						2、 硬件初始化与驱动加载
						3、 内核启动后的结局与归宿

				2、 C语言阶段分析
					1）杂碎
						smp 		对称多处理器（多核心CPU）
						lockdep		锁的依赖，是一个内核调试模块，处理内核自旋锁死锁问题相关的。
						cgroup		内核提供的一种处理进程组的技术。

					2）内核打印信息
						1、 代码位于 kernel/init/main.c中的572行。
						2、 printk函数 内核中用来从console打印信息的，类似于应用层的printf函数。内核编程不能使用标准库函数，因此不能使用printf函数，其实printk就是内核自己实现的printf。

					3）linux_banner的内容信息解析。

					4）setup_arch函数简介
						1、 cpu架构相关的一些创建过程(初始化)，实际上这个函数是用来确定我们当前内核的机器（arch、machine）的。我们Linux内核会支持一种CPU的运行，CPU + 开发板就确定了一个硬件平台，然后我们当前配置的内核就在这个平台上可以运行。之前说过的机器码就是给这个硬件平台一个固定的编码，仪表征这个平台。
						2、 当前内核支持的机器码以及硬件平台相关的一些定义都在这个函数中。
							setup_processor 	查找cpu信息。
							setup_machine		函数传参是机器码编号，machine_arch_type符号在include/generated/mach-types.h的32039-32049行。经过分析后确定这个传参值就是2456.
						3、 函数的作用是通过传入的机器码编号，找到对应这个机器码的machine_desc描述符，并且返回这个描述符的指针。
						4、 其实真正干活的函数是lookup_machine_type，找到这个函数发现在head-common.S中，真正赶回的函数是 __lookup_machine_type函数，
						5、 __lookup_machine_type函数的工作原理：内核在建立的时候就把各种CPU架构的信息组织成一个一个的machine_desc结构体实例，然后都给一个段属性 .arch.info.init ，连接的时候会保证这些描述符会被连接在一起。 __lookup_machine_type就去那个描述符所在处一次挨个遍历各个描述符，比对看机器码那个相同。

						6、 setup_arch函数进行了基本的cmdline处理
							1、 这里说的cmdline就是指的uboot给kernel传参时传递的命令行启动参数，也就是uboot的bootargs。
							2、 default_command_line：看名字是默认的命令行参数，实际上是一个全局变量字符数组，这个字符数组可以用来存东西。
							3、 CONFIG_CMDLINE:在 .config文件中定义的（可以再make menuconfig中去配置），这个表示内核的一个默认的命令行参数。
							4、 内核对cmdline的处理思路是：内核中自己维护了一个默认的cmdline（就是.config中配置的这一个），人后uboot还可以通过tag给内核传递一个cmdline。如果uboot给内核传cmdline城东则内核会有限使用uboot传递的这一个；如果uboot没有给内核传cmdline或者传参失败，则内核会使用自己默认的这个cmdline。以上说的这个处理思路就在 setup_arch函数中实现。

							5、 实验验证内核的cmdline确定
								 1、 验证思路：首先给内核配置是配置一个基本的cmdline，然后uboot启动内核时给uboot设置一个bootargs，然后启动内核看打印出来的cmdline和uboot传参时是否一样。
								 2、 在uboot中去掉bootargs，然后再次启动内核看打印出来的cmdline是否和内核中设置的默认的cmdline一样。

					5） setup_command_line
							1、 处理command_line相关的，主要是copy了一下。
					6） parse_early_param
							1、 解析cmdline传参和其他传参
							2、 解析是吧cmdline的细节解析出来。解析出来的内容解释一个字符串数组，数组中一次存放了一个个设置项信息。
								例如： 	console=ttySAC2,115200 root=/dev/mmcblk0p2 rw init=/linuxrc rootfstype=ext3
									
									console=ttySAC2,115200
									root=/dev/mmcblk0p2 rw		跟文件系统挂载节点
									init=/linuxrc
									rootfstype=ext3
							3、 这里只是进行了解析，并没有去处理。也就是说只是把长字符串解析成了短字符串，最多和内核例控制这个相应功能的变量挂钩了，但是并没有去执行。执行的代码在格子模块初始化代码部分。

					7） 杂碎
						trap_init				设置异常向量表
						trap_init				内存管理模块初始化
						sched_init				内核调度系统初始化
						early_irq_init			中断初始化
						console_init 			控制台初始化 

					总结：start_kernel函数中调用了很多的xx_init函数，全都是内核工作需要的模块的初始化函数。这些初始化之后内核就具有了一个基本的可以工作的条件。如果把内核必须成一个复杂的机器，那么start_kernel函数就是把这个机器的众多零部件组装在一起形成这个机器，让它具有可以工作的基本条件。

					8） rest_init
						1、 这个函数之前内核的基本组装已经完成。
						2、 剩下的一些工作就比较重要了，放在了一个单独的函数中，叫 rest_init。

					总结 : start_kernel 函数做的主要工作：打印了一些信息、内核工作需要的模块的初始化被一次调用（譬如内存管理、调度系统、异常处理...）、我们需要重点了解的及时setup_arch中做的2件事情：机器码架构的查找并且执行架构行管的硬件的初始化、内核传参cmdline。

				4、 rest_init
					1）rest_init函数做了那些事？
						1、 调用kernel_thread函数启动了2个内核线程，分别是：kernel_init 和 kthreadd。
						2、 调用 schedule 函数开启了内核调度系统， 从此Linux系统转起来了。
						3、 rest_inti最终调用了cpu_idle 函数结束了整个内核的启动。也就是说Linux内核最终结束与一个函数 cpu_idle。 这个函数里面坑定是死循环。
						4、 简单来说，Linux内核最终的状态是：有事干的时候去执行有意义的工作（执行各个进程任务），是在没活干的时候去死循环（实际上死循环也可以看成是一个任务）。
						5、 之前已经启动了内核调度系统，地阿杜系统会负责考评系统中所有的进程，这些进程里面只有有那个需要被运行，调度系统就睡终止cpu_idle死循环进程（空闲进程）住哪儿去执行有意义的干活的进程。这样操作系统就转起来了。

					2）什么是内核线程
						1、 进程和线程：简单来理解

					3）进程0、进程1、进程2
						1、 截止目前为止，我们一共涉及到3个内核进程/线程。
						2、 操作系统使用一个数字来表示/记录一个进程/线程的，这个数字就被成为这个进程的进程号。这个号码是从0开始分配的。因此这里涉及到的三个进程分别是Linux系统的进程0、进程1、进程2。
						3、 在Linux系统的命令行，使用ps命令可以查看当前Linux系统中运行的进程情况。
						4、 我们在ubuntu下 ps -aux 可以看到当前系统运行的所有进程，可以看出进程号是从1开始的。为什么不从0开始，因为进程0不是一个用户进程，而是属于内核进程。
						5、 三个进程
							进程0 ： 就是刚才讲过的idle进程，叫空闲进程，也就是死循环。
							进程1 ： kernel_init函数就是进程1，这个进程被称为 init 进程。
							进程2 ： kthreadd函数就是进程2，这个进程是Linux内核的守护进程。这个进程是用来保证Linux内核自己奔上能正常工	  作的。

						总结1：理解Linux启动后达到的一个稳定状态。注意去对比内核启动后的未定状态和uboot启动后的稳定状态的区别。
						总结2：理解进程和线程的概念。
						总结3：明白进程有一个进程号，从0开始。明白idle进程（idle进程是干嘛的）：进程2是kernel进程（基本明白干嘛的就行）。
						总结4：分析到此，发现后续的料都在进程1，所以后面课程会重点从进程1出发，分析之后发生的事情。

				5、 init进程 
					1、 init进程完成了从内核态向用户态的转变
						(1) 一个进程2中状态。 		
							init基础刚开始运行的时候是内核态，它属于一个内核线程，然后她自己运行了一个用户态下面的程序后把自己强行转沉了用户态。因此init进程自身玩沉了从内核态到用户态的过度，因此后续的其他进程都可以工作在用户态下面了。

						(2) 内核态下作了什么？
							1、 挂载跟文件系统，并试图找到用户态下的那个init程序。init进吧自己转成用户态就必须运行一个用户态的应用程序（这个影虎程序名字一般也叫init），要运行这个应用程序就必须得找到这个应用程序，要找到他就必须挂载跟文件系统，因为所有的应用程序都在文件系统中。
							内核源代码中的所有函数都是内核态下面的，执行任何一个都不能脱离内核态。应用程序必须不属于内核代码，这样才能保证自己是用户态。也就是说我们这里执行的这个init程序和内核不在一起，它是另外提供的。提供这个init程序的那个人就是跟文件系统。

						(3) 用户态下作了什么？
							1、 init进程大部分有意义的工作都是在用户态下进行的。init进程对我们操作系统的意义在于：其他所有的用户进程都直接或间接派生自init进程。

						(4) 如何从内核态跳跃到用户态？还能回来不？
							1、 init进在内核态下面时，通过一个函数 kernel_execve 来执行一个用户空间编译链接的应用程序就跳跃到用户态了。注意这个跳跃过程中进程号是没有改编的，所以一直是进程1。
							2、 回不来了，一但执行了init程序转台了用户态下整个操作系统就算真正的运转起来了，以后只能在用户态下工作了，用户态下想要进入内核态只有走API这一条路了。

						(5) init进程构建了用户交互界面
							1、 init进程是其他用户进程的老祖宗。Linux系统中一个进程的创建是通过其父进程创建出来的。根据这个理论只要有一个父进程就能深处一堆子孙进程。
							2、 init启动了login进程、命令行进程、shell进程
							3、 shell进程启动了其他用户进程

					总结 ： 明白init进程的作用 挂载跟文件系统 并使用kernel_execve 调到用户态。

					2、 代码分析
						(1) 打开控制台
							1、 Linux系统中每一个进程逗游自己的一个文件描述符表，表中存储的是本进程打开的文件。
							2、 Linux系统中有一个设计理念：一切皆文件。所以设备也是以文件的方式来访问的。我们要访问一个设备，就要去打开这个设备对应的文件描述。譬如/dev/fb0这个设备文件就代表LCD显示设备，/dev/buzzer代表蜂鸣器设备，/dev/console就代表控制台设备。
							3、 这里我们打开了/dev/console文件，并且复制了2次文件描述符，一共得到3个文件描述符。这三个文件描述符分别是0、1、2这三个文件描述符就是所谓的：标准输入、标准输出、标准错误。
							4、 进程1打开了三个标准输出输出错误文件，因此后续的进程1衍生出来的所有的进程默认斗剧哟这三个描述符。

						(2) 挂载跟文件系统
							1、 prepare_namespace函数中（mount_block_root）挂载跟文件系统
							2、 跟文件系统在哪里？跟文件系统的文件系统类型是什么？
								uboot通过传参高速kernel这些信息。 
								uboot传参中的 	root=/dev/mmcblk0p2 rw 这一句就是高速内核跟文件系统在哪里。
								uboot传参中的 	rootfstype=ext3 这一句就是高速kernelrootfs的类型。

							3、 如果内核挂载跟文件系统成功，则会打印出: VFS: Mounted root (ext3 filesystem) on device 179:2.
							   如果挂载跟文件系统失败，则会打印： No filesystem could mount root, tried: yaffs2

							4、 如果内核启动时挂载rootfs失败，则后面肯定没法执行了，肯定会死。内核中设置了启动失败休息5s重启的机制，因此这里自动重启，所以有时候大家看到反复重启的情况。

							5、 如果挂载跟文件系统失败，可能的原因有：
								1、 可能的原因是uboot传参错误。
								2、 rootfs烧录失败（fastboot烧录不容易出错，以前是手工烧录很容易出错）。
								3、 rootfs本身制作有问题。（尤其是自己做的rootfs，或者别人给的第一次用）

							6、 执行用户状态下的进程1程序
								1、 上面一但挂载rootfs成功，则进入rootfs中勋在应用程序的init程序，这个程序就是用户空间的进程1，找到后 run_init_process 去执行他。

								2、 我们如何确定init程序是谁？
									方法：先从uboot传参cmdline中有没有指定，

									如果有指定先执行cmdline中指定的程序。 cmdline中的 init=/linuxrc 这个技术指定rootfs中那个程序是init程序。 这里的指定方式就表示我们rootfs的根目录下有个叫linuxrc的程序，这个程序就是init程序。

									如果uboot传参cmdline中没有 init=xxx 或者cmdline中指定的这个xxx执行失败，还有备用方案，如： /sbin/init 、 /etc/init 、 /bin/init 、 /bin/sh等，最后实在不行，就打印 ： "No init found.  Try passing init= option to kernel. " "See Linux Documentation/init.txt for guidance." 然后over。

							7、 cmdlne常用参数
								1、 格式简介：
									(1) 由很多歌项目用空格隔开一次排列，每个项目都是 项目名=项目值 
									(2) 整个cmdline会被内核启动时解析，解析成一个一个项目名=项目值的字符串。
										这些字符串又会被再次解析从而影响启动过程。
									root=
									(1) 这个是用来指定跟文件系统在哪里的
									(2) 一般格式是：root=/dev/xxx（一般如果是nandflash上则/dev/mtdblock0，如果是inand/sd的话则/dev/mmcblk0p2）
									(3) 如果是nfs的rootfs，则root=/dev/nfs。 
									rootfstype=
									(1) 跟文件系统的文件系统类型，一般是jffs2、yffs2、ext3、ubi
									console=
									(1) 控制台信息声明，譬如console=/dev/ttySAC0,115200表示控制台使用串口0，波特率是115200.
									(2) 正常情况下，内核启动的时候回根据console=这个项目来初始化硬件，并且重定位console到具体的一个串口上，所以这里的传参会影响后续是否能从串口终端上接收到内核的信息。
									mem=
									(1) mem=用来指定内核当前系统的内存有多少
									init=
									(1) 用来指定进程1的程序pathname，一般都是 init=/dev/linuxrc 

									常见的cmdline介绍
										(1) console=ttySAC2,115200 root=/dev/mmcblk0p2 rw init=/linucrc rootfstype=ext3
											1、 第一种这种方式对应rootfs在SD/iNAND/Nand/Nor等物理存储器上。 这种对应产品正式出货时的情况。

										(2) root=/dev/nfs nfsroot=192.168.1.141:root/s3c2440/build_rootfs/aston_rootfs ip=192.168.1.10:192.168.1.141:192.168.1.1:255.255.255.0::eth0:off init=/linuxrc console=ttySAC0,115200
											2、 第二种这种方式对应rootfs在nfs上，这种对应我们实验室开发产品做调试的时候。

							8、 内核中架构相关代码简介
								1、 内核代码基本分3块
									(1) arch 
										cpu架构有关的代码，
									(2) drivers
										硬件的驱动。
									(3) 其他
										相同点是这些代码都和硬件无关，因此系统一直和驱动开发的时候这些代码几乎都不用关注。

								2、 架构相关的常用目录及含义
									(1) mach 	（machine architecture）
										arch/arm目录下的一个mach-xx目录就表示一类machine的定义，这类machine的共同点是都用xx这个cpu来做主芯片。（譬如mach-s5pv210这个文件里面都是s5pv210这个主芯片的开发板machine）；mach-xx目录里面的一个mach-yy.c文件中定义了一个开发板（一个开发板对应一个机器码），这个是可以扩展的
									(2) plat	（platform， 平台）
										plat在这里可以理解为SoC,也就是说这个plat目录下都是SoC里面的一些硬件（内部外设）相关的一些代码。在内核中把SoC内部相关的硬件操作代码就叫做平台设备驱动。
									(3) include
										这个include目录中的所有代码都是架构相关的头文件。（Linux内核通用头文件在内核源码树根目录下的include目录里）

									补充：
										1、 内核中的文件结构很庞大、很凌乱（不同的内核可能一个文件存放的位置是不同的），会给初学者带来一定困扰。
										2、 头文件include有好几个，譬如：
											kernel/include 									内核通用头文件
											kernel/arch/include/asm  						架构的特有头文件
											kernel/arch/arm/mach-s5pv210/include/mach 
											kernel/arch/arm/plat-s5p/include/plat 

										3、 内核中包含头文件有固定的格式
											#include <linux/kernel.h>		kernel/include/linux/kernel.h 
											#include <asm/mach/arch.h>		kernel/arch/arm/include/asm/mach/arch.h 
											#include <asm/setup.h> 			kernel/arch/arm/include/asm/setup.h 
											#include <plat/s5pv210.h>			kernel/arch/arm/plat-s5p/include/plat/s5pv210.h 
										4、 有些同名的头文件是由包含关系的，有时候我们需要包含某个头文件时可能并不是直接包含他，而是包含一个包含了他的头文件。

				三、三星官方kernel移植体验
					1、 初步编译烧写问题:	start kernel... 后没有任何输出

					2、 分析问题： 
						(1) 根据运行结果，分析发现：Linux内核的自解压代码都没有运行（因为没有看到： Uncompressing Linux... done, booting the kernel,）
						(2) 说明zImage根本没有被解压成功，内核代码根本就没有被运行，当然没有输出信息了。所以问题出在解压相关部分。
						(3) 问题出在内核配置的解压后代码防止的内存地址处。
						(4) 内核配置的解压地址应该等于链接地址，否则自解压之后内核无法运行。现在问题变成：第一，内核的链接地址等于多少？第二，内核中配置的解压地址是多少？
						(5) 这里main还有个问题：内核的链接地址是一个虚拟地址，儿子解压代码解压内核时需要物理地址，因此上面说的等于，其实是链接地址对应的物理地址等于自解压地址。
						(6) 链接地址和他对应的物理地址在 head.S中可知： 物理地址 ：30008000，虚拟地址 ： c0008000
						(7) 自解压代码对应的自解压地址在mach/Makefile.boot文件中。在其中修改增加 SMDKV210支持相关代码

						(8) 同步编译烧写后自解压打印信息出现但是仍没有启动成功。

					3、 内核中机器码的确定
						(1) MACHINE_START红
							1、 这个宏来定义一个机器码的数据结构的。 这个宏的使用其实是用来定义一个结构体类型为 machine_desc 类型的结构体变量，名为 __mach_desc_SMDKV210 。这个结构体变量会被定义到一个特定的段 .arch.info.init ，因此这个结构体变量会被链接器连接到 .arch.info.init 段中。

								static const struct machine_desc __mach_desc_SMDKV210	\
								 __used							\
								 __attribute__((__section__(".arch.info.init"))) = {	\
									.nr		= MACH_TYPE_SMDKV210,		\
									.name		= "SMDKV210",
									/* Maintainer: Kukjin Kim <kgene.kim@samsung.com> */
									.phys_io	= S3C_PA_UART & 0xfff00000,
									.io_pg_offst	= (((u32)S3C_VA_UART) >> 18) & 0xfffc,
									.boot_params	= S5P_PA_SDRAM + 0x100,
									.init_irq	= s5pv210_init_irq,
									.map_io		= smdkv210_map_io,
									.init_machine	= smdkv210_machine_init,
									.timer		= &s5p_systimer,
								};

							2、 经过分析：发现一个mach-xxx.c文件中定义了一个机器码的开发板的machine_desc结构体变量，这个结构体变量放到 .arch.info.init 段中后，那么就表示当前内核可以支持这个机器码的开发板。
							3、 当前开发版和当前内核中来分析，当前我们使用的开发板时S5PV210的CPU，开发板名字X210,我们在三星官方版本的内核中是找不到mach-x210.c的，所以我们又不想从零开始去移植，因此我们的思路是在三星移植的mach-s5pv210目录下找一个mach-xx.c，这个开发板和我们的X210开发板最为接近，然后以此为基础来移植。
							4、 经过查看，法相mach-s5pc110.c 和 mach-smdkv210.c和我们的最相似。我们一般确定的一个原则是：看我们的开发板和三星官方的那个开发板最为相似。我们的x210炒的是三星SMDKV210，因此要找这个对应的那个文件。
							5、 结合mach-s5pv210目录下的Makefile来分析，得知 .config 中定义了CONFIG_MACH_SMDKV210后，实际绑定的事mach-smdkc110.c这个文件。所以实际上mach-smdkv210.c这个文件根本没用到。启示就是不要光看名字。

						(2) 硬件驱动的加载和初始化函数执行
							1、 .init_machine	= smdkc110_machine_init, 这个函数
								这个元素定义了一个机器硬件初始化函数，这个函数非常重要，这个函数中绑定了我们这个开发板Linux内核启动过程中会初始化的各种硬件的信息。

						(3) 解决内核启动中的错误
							1、 认识内核启动OOPS
								(1) 内核启动后会有打印信息，打印信息中隐藏了问题所在。认真的去分析这个额打印信息，从中找到对的或错误的一些信息片段，才能帮助我们找到问题，从而解决问题。

								(2) 内核启动中的错误信息有一些特征：
									Unable to handle kernel NULL pointer dereference at virtual address 00000060
									Internal error: Oops: 5 [#1] PREEMPT
									PC is at dev_driver_string+0xc/0x44
									LR is at max8698_pmic_probe+0x150/0x32c

								(3) 从以上错误信息中的PC和LR的值可以看出，程序是执行到 dev_driver_string 和 max8698_pmic_probe （这两个势函数或者汇编中的标号）符号部分的时候出错了。我们就从这两个符号出发去寻找、思考可能出错的地方然后试图去解决。

							2、 错误追溯及问题解决
								(1) max8698_pmic_probe 看名字是max8698这个电源管理IC的驱动安装函数部分出错了，应该使我们的开发板系统中配置了支持这个电源管理IC，于是乎启动时区加载他的驱动，结果驱动在加载执行的过程中出错了OOPS了。

								(2) 我们为什么要配置支持这个驱动？这个驱动加载为什么要出错？

								(3) 结合我们x210开发板的硬件实际情况来分析：我们x210开发板根本就没有max8698这个电源管理IC，既然硬件都没有驱动执行了肯定会出错。

								(4) 回忆三星版本的uboot移植的时候，在uboot的lowlevel_init.S 中也有调用这个电源管理IC初始化函数（PMIC_init），后来解决的办法就是屏蔽掉这个函数的调用，uboot九成宫运行下去了。

								(5) 为什么uboot和kernel中都有调用PMIC呢，因为三星原厂的是由PMIC的。

								(6) 解决办法，去 make menuconfig中去掉 MAX8698相关的配置。再编译烧写运行。

								(7) 实践得知，内核运行了知道跟文件系统才出错。

							3、 分析及总结
								(1) 分析：问题究竟是怎么被解决的？设计那几个方面
									根本原因在于 CONFIG_MFD_MAX8698 这个配置宏。这个配置宏决定了很多东西。
									第一：这个宏决定了drivers/mfd目录下的 max8698.c 的驱动源码被编译
									第二：这个配置宏还决定了kernel启动过程中是否会调用一些max8698的相关的宏。
								(2) 总结： kernel是高度模块化和可配置化的，所以在内核中做任何做事情（添加一个模块、更改一个模块、去掉一个模块）都必须按照内核设定的方案和流程来走。

						(4) iNand的问题和安排
							1、 错误分析
								(1) 错误信息
									Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0)
									从错误信息字面信息分析，就是内核试图挂载跟文件系统时失败，失败原因是 unknown-block （不能识别的块设备）

								(2) backstrace分析，可以得知错误信息的来源，在饥饿和之前的内核启动流程分析，就更加确定了出错的地方。

								(3) 下一个问题：分析这个错误出现的原因。 unknown-block(0,0) 。 在kernel启动是uboot回传给内核一个cmdline，启动用root=xxx来指定rotfs在那个设备上，内核就会到相应的地方挂在rotfs。譬如我们传参中：root=/dev/mmcblk0p2 rw，这里的/dev/mmcblk0p2就是rootfs的设备地址，这个设备文件便函的含义就是mmc设备0的第2个分区（设备0就是在SD0通道上的设备，也就是iNand），这里的问题就是没找到mmc设备0的第2分区。

								(4) 下一个问题：为什么没找到mmc设备0的第2分区？		一定是kernel启动过程中加载mmc驱动的时候有问题，驱动没有发现mmc设备0.问题定位在MMC相关的驱动。

								(5) 对比九鼎版本的内核启动信息，即可发现我们的内核启动并没有找到MMC设备（内置iNand和外置的SD卡都没有找到），没找到可能是驱动的问题，这就要取一只MMC驱动了。

							2、 问题阐述
								(1) SD/iNand本身都是由一个一个的扇区组成的，回忆裸机中讲到的210的启动时，BL1在SD卡的1扇区开始往后存放的，SD卡的0扇区是不用的。SD卡的0扇区是用来防止MBR的
								(2) MBR就是用来描述块设备的分区信息的，实现定义了一个通用的数据结构来描述块设备的分区，我们只要按照这个标准将分区信息写入MBR中即可对该设备完成分区。MBR默认就是在块设备的第0个扇区上存放的。
								(3) 我们内核中读到iNand分4个分区，我们那里分区的？ 		uboot中国有一个命令fdisk -c 0 时就对iNand进行了分区。uboot的fdisk命令内部已经写死了iNand的分区表，到内核中时内核直接读取MBR就知道了分区。所以在uboot和内核之间iNand设备的分区信息是靠iNand自己传递的，所以uboot不用给内核传参时传递分区信息。

							3、 解决安排
								(1) 暂时解决不了这个问题。

							4、 后续课程安排。
								(1) 一节课搞定网卡，一节课讲述内核移植的小技巧，然后结束。


						(5) 网卡驱动移植 
							成功： 
								[    1.452008] dm9000 Ethernet Driver, V1.31
								[    1.455870] eth0: dm9000c at e08f4300,e08f8304 IRQ 42 MAC: 00:09:c0:ff:ec:48 (platform data)
							我们移植的kernel中的打印信息：
								[    1.134263] dm9000 Ethernet Driver, V1.31
								[    1.137061] ERROR : resetting 
								[    1.139655] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.144865] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.149891] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.154913] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.159942] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.164965] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.169994] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.175017] dm9000 dm9000.0: read wrong id 0x2b2a2928
								[    1.180045] dm9000 dm9000.0: wrong id: 0x2b2a2928
								[    1.184721] dm9000 dm9000.0: not found (-19).

							目标：我们移植版本的内核打印正确的信息。

							1、 make menuconfig总添加DM9000支持
								内核默认支持。
							2、 mach-smdkc110.c中逻辑分析
								(1) mach-smdkc110.c中的smdkc110_machine_init是整个开发板的所有硬件的初始化函数，在这里加载了的硬件将来启动时就会被初始化，在这里没有的将来启动时就不管。
								(2) smdkc110_devices 和 smdkc110_dm9000_set() 这两个地方是和DM9000有关的，要分别去做移植。

								(3) smdkc110_dm9000_set() 这个函数就是 DM9000 相关的SROM bank的寄存器设置，相当于uboot中dm9000 移植时的dm9000_pre_init函数。只是读写寄存器的函数不一样。

							3、 修改相应的配置参数
								(1) 在 arch/arm/plat-s5p/devs.c 中更改
								(2) 在 arch/arm/mach-s5pv210/include/mach/map.h中定义了DM9000的IO基地址，和DM9000接在那个bank有关。
								(3) 还有+2改成+4，
							4、 代码实践

						(6) 内核启动第一阶段的调优方法
							1、 问题点描述
								(1) 内核启动在head.S中进行了三个校验（CPU id校验、机器码的校验、tag的校验），然后创建页表，然后做了一些不太会出错的事情，然后b start_kernel。基本上能运行到start_kernel内核移植就不太会出问题了。
								(2) 有时候移植的内核启动后的现象是：根本没有启动信息出来。这时候有可能是内核启动运行了但是运行出错了没启动起来所以没有打印信息；也有可能是内核根本没得运行。都有可能但是没法确定。我们希望能有一种调试手段来确定问题所在。

							2、 调试方法和原理
								(1) 调试方法就是在内核启动的第一阶段添加汇编操作led点亮/熄灭的方法来标明代码运行轨迹。
								(2) 找之前逻辑汇编操作led点亮/熄灭的代码过来，复制粘贴到head.S中合适位置。然后内核启动后根据led表现来标明代码有误运行。
							3、 动手测试
								(1) 整理好led啊哦做的代码段，在head.S中合适的地方添加led这个函数，然后在head.S的内核的起始阶段添加调用led函数，然后重新编译内核，运行内核看这段代码有误被执行。
								(2) 如果被运行了，证明在这个调用led的步骤之前的部分都是没问题的，那么如果有错肯定错误在后边；如果没有被运行则证明错误在之前，那么就要去之前的部分debug了。
							4、 实验验证



				四、文件系统
					1、自己制作ext3格式的跟文件系统
						(1) mke2fs介绍
							1）mke2fs是一个应用程序，ubuntu默认安装了，作用：用来制作ext2、ext3、ext4等格式的更文件系统的。
							2）一般用来制作各种不同格式的rootfs的应用程序的名字都很相似，类似于 mkfs.xx（譬如：mkfs.ext2、 mkfs.jffs2）
							3）Ubuntu20.04中的mkfs.ext2等都是mke2fs的符号链接而已。

						(2) 动手制作ext3格式的跟文件系统
								《参考：https://blog.csdn.net/zhengmeifu/article/details/24174513》
							创建rootfs.ext2文件并且降至关在到一个目录下方便访问它	
							1）dd if=/dev/zero of=rootfs.ext2 bs=1024 count=2048
							2) losetup /dev/loop1 rootfs.ext2
							3）mke2fs -m 0 /dev/loop1 2048
							4）mount -t ext2 /dev/loop1 ./rootfs/

							2、 我们向镜像中写入一个普通文件linuxrc。这个文件就会成为我们制作的镜像中的/linuxrc。内核挂载了这个镜像后就会尝试去执行/linuxrc。然后执行时必然会失败。我们将来实验看到的现象就应该是：挂载成功，执行/linuxrc失败。

							3、 将来真正去做有用的rootfs时，就要在这一步添加真正可以执行的linuxrc程序，然后还要添加别的/lib目录下的库文件，/etc目录下的配置文件等。

							4、 卸载，然后镜像就做好了
								umount /dev/loop1
								losetup -d /dev/loop1

					2、 NFS方式启动资质简易文件夹形式的rootfs
						(1) nfs启动方式的bootargs
							setenv bootargs root=/dev/nfs rw nfsroot=192.168.1.11:/home/strudy1/sambashare/linux_study/uboot_kernel/rootfs/rootfs,v4 ip=192.168.1.20:192.168.1.11:192.168.1.1:255.255.255.0::eth0:off init=/linuxrc console=ttySAC2,115200

						注意：按照老朱的视频会报错因为 需要制定 nfs 版本。如上 v3
							开发板在访问nfs服务器时，会以服务器版本为准。如果开发板配置支持v3，而服务器为v4，这时候默认是v4版本，所以就会出错。

					3、到底什么是Linuxrc
						(1) /linuxrc是一个可执行的应用程序
							1）/linuxrc是应用层的，和内核源码一点关系都没有
							2）/linuxrc在开发版当前内核系统下是可执行的。因此ARM SoC的Linux系统下，这个应用程序就是arm-linux-gcc编译连接的；如果实在PC机 Linux系统下，那么这个程序就是用gcc编译连接的。
							3）/linuxrc如果是静态编译连接的那么直接可以运行；如果是动态编译连接的那么我们还必须提供必要的库文件才能运行。但是因为/linuxrc这个程序时由内核海子街调用执行的，因此用户没有机会去导出库文件的路径，因此实际上这个/Linuxrc没法动态链接，一般是静态静态链接的。

						(2) /linuxrc执行时引出用户界面
							1）操作系统启动后在一系列的自己运行配置后，最终会给用户一个操作界面（也许是cmdline，也许是GUI），这个用户操作接麦你就是 /linuxrc 带出来的。
							2）用户界面等很多事并不是在 /linuxrc 程序中负责的，用户界面有自己专门的应用程序，但是用户界面的应用程序是直接或间接的被 /linuxrc 调用执行的。用户界面程序和其他的应用程序就是进程 2、3、4... ，这就是我们说的进程1 （init进程，也就是 /linuxrc ）是其他所有应用程序进程的祖宗进程。

						(3) /Linuxrc负责系统启动后的配置
							1）就好像一个房子建好之后不能直接住，还要装修一样；操作系统启动起来之后也不能直接用，要配置下。
							2）操作系统西东后的应用层的配置（一般叫运行时配置，英文简写 etc ）是为了让我们的操作系统运行起来更方便，更适合我们个人的爱好或者实用性。

						(4) /linuxrc在嵌入式Linux中一般就是busybox
							1）busybox是一个C语言写出来的项目，里面包含了很多.c文件和.h文件。这个项目可以被配置编译成各个平台下面可以运行的应用程序。我们如果用arm-linux-gcc来编译busybox就会得到一个可以再开发板Linux内核上运行的应用程序。
							2）busybox这个开发出来就是为了在嵌入式环境下构建 rootfs 使用的，也就是说他就是专门开发的init进程应用程序。
							3）busybox为当前系统提供了一整套的 shell 命令程序集。譬如vi、cd、ls、mkdir等。在桌面版的Linux发行版（譬如：ubuntu、redhat、centos）中vi、cd、ls等都是一个一个的单独的应用程序。但是在嵌入式Linux中，为了省事我们把vi、cd等所有常用的shell命令集合到一起构成了一个shell命令包，起名叫busybox。

					4、 rootfs还需要有什么？
						(1) /linuxrc
						(2) /bin  普通用户使用的。		/sbin 	操作系统管理员用的。 
						(3) lost+found  回收站
						(4) /dev  设备文件。 一切皆文件，因此一个硬件设备也被虚拟化成一个设备文件来访问，在Linux系统中/dev/xxx就表示一个硬件设备，我们要操作硬件就是 open 打开这个设备文件，然后 read/write/ioctl 操作这个设备，最后 close 关闭这个设备。	在最小 rootfs 中 /dev目录也是不可少的，这里面有一两个文件是 rootfs 必须的。
						(5) /sys 和 /proc 目录。 在最小rootfs中也是不可省略的。但是这两个只要创建了空文件即可，里面是没东西的，也不用有东西。这两个目录也是和驱动有关的。属于Linux中的虚拟文件系统。

						(6) /usr 目录。 系统用户所有的一些文件的存放地，这个东西将来busybox安装时会自动生辰。
						(7) /etc 目录是很关键很重要的一个。 目录总的所有文件全部都是运行时配置文件。/etc 目录下的所有配置文件会直接或间接的被 /linuxrc 所调用执行，完成操作系统的运行时配置。 etc目录是制作 rootfs 的关键，是有意后面下一个课程专门讲 etc 目录。
						(8) /lib 目录。 rootfs 中很关键的一个，不能省略。 lib目录下放的是当前操作系统中的动态和静态库。 我们主要是为了其中的动态链接库。

					5、 VFS 简介
						(1) 什么是VFS?
							1）VFS是Linux内核的一种设计理念、设计机制。 VFS就是 vitrual file system，即：虚拟文件系统。
							2）具体的一些文件系统 FAT、NTFS、ext2、ext3、jffs2、yaffs2、ubi等主要设计目的是为了管理块设备（硬盘、Nand...）
							3）VFS是借鉴了文件系统的设计理念（通过文件系统将系统底层难以管理的物理磁盘扇区式访问，转换成目录+文件名的方式来访问），将硬件设备的访问也虚拟化成了对目录 + 文件的访问。 所以有了VFS后我们可以通过设备文件（目录 + 文件名，譬如：/dev/mmcblk0p2）的方式来访问系统中的硬件设备。
							4）以上可以初步看出VFS的一些厉害之处，但是VFS不止于此。

						(2) VFS的意义
							1）上面说过的对硬件设备的访问，将对硬件设备的访问核对普通文件的访问给接口统一化了。（Linux中一切皆文件）
							2）将操作系统上层（应用层）对下层不同文件系统类型的访问细节给屏蔽掉了。因此如果没有VFS那我们写cp命令（其他命令也一样）的时候就不得不去考虑cp的这个文件在什么文件系统类型下。所以 cp 命令非常复杂，因此要考虑具体的文件系统类型。有了VFS后情况就不一样了。VFS成了一个隔离层，隔离了下层的不同文件系统的差异性，对上层应用提供一个统一的接口。
							3）VFS 将不同文件系统和下层硬件设备（块设备）驱动之间的细节也给屏蔽了。不同类型的文件系统在本省设计时是不用考虑各种不同的硬件设备的具体操作差异，这列有一个类似 VFS 的设计理念。

						(3) VFS和我们学习的关系
							1）VFS 机制和 rootfs 挂载，和其他文件系统的挂载都是有关联的。
							2）内核中有一些 sys proc 这种虚拟文件系统，这些东西也是和VFS机制有关。
							3）/dev/目录下的设备文件都和 VFS 有关，所以学习驱动绕不开。

				五、rootfs 的构建
					1、 busybox的移植
						1）busybox源代码下载，busybox是一个开源项目，直接在网上下。
						2）busybox的版本差异不大，版本新旧无所谓。

						1、 修改 Makefile 
							ARCH = arm
							CROSS_COMPILE = ...

						2、 make menuconfig 配置 

						3、 make 然后 make install
							1、 make编译，如果有错就解决之
							2、 make install执行的时候其实就是在执行busybox顶层目录下的一个install。
								make install 在所有的Linux下的软件中作用都是安装软件。在传统的Linux系统中安装软件时都是选择代码方式安装的。我们下载要安装的软件源代码，然后配置、编译、安装。make install 的目的就是将编译生成的可执行西程序及其依赖的库文件、配置文件、头文件安装到当前系统中指定（一般都可以自己指定安装到那个目录下，如果不指定一般都有个默认目录）的目录下。

						4、 设置bootargs挂载添加busybox移植的rootfs
							1、 之前建立了一个空的文件夹然后自己touch linuxrc随便创建了一个能用的 /linuxrc 然后去nfs挂载rootfs，实验结果是：挂载成功，执行 /linuxrc 失败。 
							2、 现在我们移植了busybox后 /Linuxrc 就可以用了，然后再次去nfs挂载这个rootfs。 预计看到的效果是：挂载成功，执行 /linuxrc 也能成功。

							3、 注意设置bootargs

							4、 实验结果：挂载成功，执行 /linuxrc （也就是busybox）成功，但是因为找不到 /etc/init.d/rcS 和 /dev/tty2 等文件所以一直在打印错误提示信息，但是其实又进入命令行。

					2、 inittab详解
						(1) 添加一个典型的inittab
							1、 将我们提供的典型的 inittab 文件复制到我们制作的 rootfs 的根目录下的 /etc/ 目录下 
							2、 再次启动内核挂载这个 rootfs 看效果。

							3、 实验现象是成功启动并挂在了 rootfs 进入了控制台命令行。当前制作的最小 rootfs 成功了。

						(2) inittab格式详解
							1、 inittab的工作原理就是 /linuxrc （也就是busybox）执行时所调用起作用。
							2、 inittab在 /etc 目录下，所以属于一个运行时配置文件，是文本格式的（内容是由一系列的最找一个格式组织的字符组成的），实际工作的时候 busybox 会（按照一定的格式）解析这个inittab文本文件，然后根据解析的内容决定要怎么工作。
							3、 busybox究竟如何完成解析并且解析结果如何去工作（busybox中实现 /etc/inittab的原理）并不是我们的目标，我们的重点是 inittab 的格式究竟怎样的？我们看到一个inittab后怎么去分析这个 inittab 对启动的影响。
							4、 inittab的格式在busybox中定义的，网上很多可以参考。
								第一个： #开始的行表示注释
								第二个： ： 表示分隔符，分隔开各个部分
								第三个：inittab内容是一行为单位的，行与行之间没有关联，每行都是一个独立的配置项，每一个配置项表示一个具体的含义。
								第四个：每一行的配置项都是由 三个 ： 分隔开的四个配置值共同确定的。这四个配置值就是 
										id:runlevel:action:process。	
										值得注意的是有些配置值可以空缺，空缺后冒号不能空缺，所以有时候可以看到连续两个冒号。
								第五个： 每一行的配置项中4个配置值最重要的是 action和process，action是一个条件/状态， process是一个可被执行的path那么。和其阿里的意思就是：当瞒住action的条件时就会执行 process 这个技术指定rootfs中那个程序是init程序。
								注意：理解inittab的关键就是明白“当瞒住action的条件时就会执行process这个程序。”你去分析 busybox 的源代码就会发现，busybox最终会进入一个死循环，在这个死循环中去反复检查是否瞒住各个 action 的条件，如果某个 action 的条件满足就会去执行对应的 process。
								第六个： 明白各个action什么意思。

						(3) busybox相关源码分析
							1、 源码目录梳理

							2、 SourceInsight工程建立

							3、 整个工程入口确认
								1）分析一个程序，不管多庞大还是小，最好的路线都是按照程序运行时的逻辑顺序来。所以找到一个程序的入口至关重要。
								2）学C语言的时候都知道程序的主函数main函数就是整个程序的入口
								3）在uboot和Linux kernel这两个大的长元音的项目中，main函数都没有，都不是入口。在我们这种逻辑程序中入口不是main函数，而是有链接脚本来指定的。
								4）busybox是Linux启动起来后工作的一个应用程序，因此其中必然有main函数，而且main就是入口。

							4、 busybox中main函数全解析
								1）busybox入口就是main函数，其中由很多个 main 但是只有一个起作用了，其他的没有起作用。真正busybox工作时的入口是libbb/appletlib.c中的main函数。 
								2）busybox中很多xxx_main函数，这些main函数每一个都是busybox支持的一个命令的真正入口。譬如 ls_main 函数就是busybox当做 ls 函数使用时的入口函数程序。
								3）ls、cd等命令其实都是busybox一个程序，但是实际执行时的效果确实格子的效果。busybox是如何实现一个程序化身万千还能各自工作的？答案就是 main 转 xxx_main。也就是busybox每次执行都是先执行其 main，在main函数中识别（靠main函数的传参argv[0]来识别）我们真正要执行的函数（譬如：ls）然后去调用相应的xxx_main（譬如：ls_main）来具体实现这个命令。

							5、 inittab解析与执行
								1）inittab的解析是在 busybox/init/init.c/init_main函数中。
								2）函数的执行逻辑是：先通过 parse_inittab 函数解析 /etc/inittab （解析的重点是将inittab中的各个action和process解析出来），然后后面先执行sysinit和wait和once（注意这里只执行一遍），然后在 while(1) 死循环中去执行respawn和askfirst。

							6、 pwd命令执行路径分析
								1）根据以上分析知，在busybox命令行下执行pwd命令时实际执行的是 pwd_main 这个函数。

							7、 busybox的体积优势原理
								1）busybox实际上就是把ls、cd、mkdir等很多个Linux中常用的shell命令的集成在一起了。集成在一起后有一个提及优势：就是busybox程序的大小比 busybox 中实现的那些命令的大小假期阿里要小很多。
								2）busybox变小的原因主要有两个：第一个busybox本身提供的shell命令时阉割版的（busybox中的命令支持的参数选项比发行版中要少，譬如 ls 在发行韩中可以有几十个 -x，但是在busybox中只保留了几个常用的选项，不常用的都删除掉了）；第二个是 busybox 中因为所有的命令的实现代码都是一个程序中实现的，而各个命令中有很多底阿妈函数都是通过的（譬如 ls、cd、mkdir等命令都会去操作目录，因此在busybox中实现目录操作的函数就可以被这些命令共用。）共用会降低重复代码出现的次数，从而减少总的代码量的体积。
								3）经过分析，busybox的体积优势是嵌入式系统本身的要求和特点造成的。

						(4) rcS文件介绍
							1、 /etc/init.d/rcS 文件时 Linux 的运行时配置文件中最重要的一个，其他的一些配置都是由这个文件印出来的。这个文件可以很复杂，也可以很简单，里面可以有很多的配置项。

							2、 PATH=xxx 
								1）首先冲shell脚本的语法角度分析，这一行定义了一个变量 PATH ，值等于后面的字符串
								2）后面 export 导出了这个 PATH ，那么 PATH 就变成了一个环境变量。
								3）PATH 这个环境变量时 Linux 系统内部定义的一个环境变量，含义是操作系统去执行程序时会默认到 PATH 指定的各个目录下去寻找。如果找不到就认定这个程序不存在，如果找到了就去执行它。讲一个可执行程序的目录导出到 PATH,可以让我们不带路径来执行这个程序。
								4）rcS中问什么要先到处 PATH？ 就是因为我们希望一旦进入命令行下时，PATH环境变量中就有磨人的 /bin /sbin /usr/bin /usr/sbin 这几个常见的可执行程序的路径，这样我们进入命令行后就可以 ls、cd等直接使用了。
								5）为什么RCS文件还没添加，系统启动就有了 PATH 中的值? 原因在于 busybox 自己用代码硬编码为我们导出了一些环境变量，其中就有 PATH。

							3、 runlevel= 
								1）runlevel屙屎一个环境变量，并且被导出为环境变量。
								2）runlevel这个环境变量到底有什么用？
									见 百度。
								3）runlevel=S 代表设置系统为单用户模式。

							4、 prevlevel=

							5、 umask=
								1）umask是Linux的一个命令，作用是设置Linux系统的umask值。
								2）umask值决定当前用户在创建文件时的默认权限。

							6、 mount -a
								1）mount命令时用来挂载文件系统的
								2）mount -a 是挂载所有的应该被挂载的文件系统，在busybox中mount -a时 busybox 回去查找一个文件 /etc/fstab 文件，这个文件按照一定格式列出所有应该被挂载的文件系统（包括虚拟文件系统）

						(5) RCS文件 测试
							1、 umask=022 则默认权限为 644   对应为相加为 666

						(6) mount测试
							1、 挂载时全部出错：
								mount: mounting proc on /proc failed: No such file or directory
								mount: mounting sysfs on /sys failed: No such file or directory
								mount: mounting tmpfs on /var failed: No such file or directory
								mount: mounting tmpfs on /tmp failed: No such file or directory
								mount: mounting tmpfs on /dev failed: No such file or directory
							2、 原因是因为跟文件系统中找不到挂载点。所谓挂载点就是我们要将目标文件系统（当然这里都是虚拟文件系统）挂载到当前文件系统中的某一个目录中，这个目录就是挂载点。
							3、 解决方案就是自己在制作的rootfs 根目录下创建这些挂载点目录即可。
							4、 验证挂载是否成功，可以看挂载信息；还可以看 proc和sys目录下是否有东西。

						(7) mdev 
							1、 mdev是udev的嵌入式简化版本，udev/mdev是用来配合 Linux 驱动工作的一个应用层的软件，udev/mdev的工作就是配合Linux驱动生成相应的 /dev 目录下的设备文件。
							2、 这里涉及驱动，因此详细的需要配合驱动。

						(8) hostname
							1、 是Linux中的一个命令。命令（hostname xxx）可以设置当前系统的主机名，直接hostname 可以查询主机名。

						(9) profile文件和用户登录理论
							1、 profile文件添加
								1）之前添加了 /bin/hostname 在/etc/sysconfig/HOSTNAME 文件中定义了一个 hostname （Master_r），实际效果是：命令行下 hostname 可以查到，但是命令行的提示符不显示。
								2）这个问题的解决就要靠 profile文件。 将profile 文件放在 etc目录下即可。

								3）添加了实验现象，命令行前面显示 [#Master_r]#

								结论：profile文件起作用了，hostname 显示出来了。但是登陆用户名没有线束出来。

								4、 profile文件工作原理：profile是被busybox（init进程）自动调用的，认名字的。
							2、 如何看到用户登录界面
								1、 Linux中有一个原则就是用一个小程序来完成一个功能。如果我们产品确实需要很复杂的总和型的功能，我们倾向于先试用很多个小程序完成其中一个功能，然后再将这些小程序集成起来完成整个大功能的产品。
								2、 这种集成很多小程序完成一个大的功能，有很多种技术实现。譬如shell脚本，还有一些别的技术，譬如Linux启动中的inittab。
								3、 因为之前 inittab中有一个配置项 ：：askfirst：-/bin/sh，这个配置项作用就是当系统启动后就去执行/bin/sh，执行这个就会出现命令行。
								4、 要出现登录界面，就不能直接执行/bin/sh，而应该执行一个负责出现登录界面并且负责管理用户名和密码的一个程序，busybox中也继承了这个程序（就是/bin/login 和 /sbin/gettty），因此我们要在inittab中用/bin/ligin或者 /sbin/gettty去代替/bin/sh。

							3、 用户名和密码的设置
								1、 用户名和密码的设置和登录程序有关联的，但是 /bin/login 和 /sbin/gettty 在用户名和密码的管理上市一样的。其实常见的所有Linux系统的用户名和密码的管理几乎都是一样的。
								2、密码一般都是加密文字的，而不是用明文。意思就是系统中的密码肯定是在系统中一个专门用来存密码的文件中存储的，用明文存密码有风险，因此Linux系统都是用 密文存储密码的。

						(10) 用户登录实战
							1、 添加/bin/login到sysinit
								1、 在inittab中修改，去掉/bin/sh，换上/bin/login，则系统启动后出现登录界面。可以输入用户名和密码。
								2、 实验现象：可以输入用户名，但是密码一直不对。

							2、 添加passwd和shadow文件
								1、 为什么用户名和密码不对？因为我们根本没设置root用户的密码。
								2、 Linux系统中用来描述用户名和密码的文件时passwd和shadow文件，这两个文件都在etc目录下。passwd文件中存储的是用户的密码设置，shawd文件中存储的是加密后的密码。
								3、 直接复制ubuntu系统中的 /etc/passwd和/etc/shadow文件到当前制作的rootfs目录下，然后再做修改即可。
								4、 /etc/passwd和/etc/shadow修改好厚，shadow总默认有一个加密的密码口令，这个口令和拷贝的shadow有关，这里可以直接删除，在进入Linux系统后自己使用 passwd xxx。

							3、 重置密码实践
								1、 ubuntu刚装好的时候默认登录是用普通用户登录的，默认root用户是关闭的。普通用户的密码是在装系统的时候设置的，普通用户登录后可以使用 su passwd root 给 root 用户设置密码，设置了密码后 root 用户才可以登录。
								2、 其实这个原因就是 root 用户在 /etc/shadow 文件中加密口令是空白的。所以是不能登陆的。
								3、 busybox 中因为没有普通用户，所以做法是：默认 root 用户如果加密口令是空的则默认无密码直接登录。等我们登陆了之后还是可以用passwd root 给 root 用户设置密码。
								4、 平时有时候我们忘记了自己的操作系统的密码，怎么办？有一种解决方法就是用其他系统（WindowsPE系统或者ubuntu的单用户模式等...）来引导启动，启动后挂载到我们的硬盘上，然后找到/etc/shadow文件，去掉密文密码后保存，然后在重启系统。

							4、 getty实战
								1、 后面做项目会发现，inittab中最常见的用于登录的程序不是/bin/login反而是/sbin/getty。
								2、 在busybox中这两个是一样的。这两个其实都是busybox的符号链接而已。因此不用严格区分这两个。
								3、 可以在inittab中直接用 /sbin/getty -L xxx 115200 vt100 直接代替 /bin/login，效果一样。


						(11) 动态链接库的拷贝
							1、 静态编译helloworld程序并执行。
								1）写一个helloworld，交叉编译，拷贝过去并执行。

								2）静态链接的可以执行，但是动态链接的不能执行，原因是开发板环境中没有相应的动态库。

								3）所以应该将 arm-Linux-gcc 的动态库文件复制到开发板 rootfs 的/Lib目录下即可解决。

								4）动态链接库so文件包含了吊事符号信息，这些符号信息在运行中是没用的（调试时用的），这些符号会占用一定空间。在传统的嵌入式系统中flash空间是有限的，为了节省空间常把这些符号信息去掉。这样节省空间并且不影响运行。 去掉符号命令： arm-linux-strip *so*

							2、 

						(12) 开机自启动与主流的rcS格式介绍
							1、 修改rcS实现开机自启动
								1）开机起启动指的是让一些应用程序能够开机自动执行
								2）开机自启动的实现原理就是在开机会自动执行的脚本RCS中添加上执行某个程序的语句代码即可。

							2、 前台运行与后台运行
								1）前台运行：程序运行时占用了当前控制台，程序不结束控制台就无法使用（独占）
								2）后台运行：程序运行但是不占用当前控制台，不影响控制台的其他操作。

							3、 开机装在驱动等其他开机自启动执行

							4、 实际开发中 rootfs 的RCS是怎样的
								1）rcS 会引入其他文件

						(13) 制作ext2格式的影响并烧录启动
							1、 确定文件夹格式的 rootfs 可用 
								1）用作后面对比

							2、 动手制作制作ext2格式的镜像
								制作：
									1）dd if=/dev/zero of=rootfs.ext2 bs=1024 count=2048
									2）losetup /dev/loop1 rootfs.ext2
									3）mke2fs -m 0 /dev/loop1 2048
									4）mount -t ext2 /dev/loop1 ./rootfs/

								卸载：
									1）umount /dev/loop1
									2）losetup -d /dev/loop1

							3、 烧录镜像并设置合适的bootargs
								1）使用 fastboot 命令： fastboot flash system xxx 
							4、 总结 

				六、 X210的bsp
					1）x210的bsp介绍
						1、嵌入式Linux产品的bsp介绍
							1 大部分的ARM架构的Linux平台的bsp的内容和结构都是相似的。
							2 bsp一般是芯片厂家/办卡厂家提供的。

						2、 X210的linux+QT bsp整体介绍
							1 tslib_x210_qtopia.tgz是用来支持QT的触摸屏操作的应用层库。
							2 xboot和uboot是X210支持的2个bootloader源代码。kernel是内核源码。buildroot是用来制作跟文件系统的文件夹。tools是一些应用工具。
							3 mk脚本是用来管理和编译bsp的。

						3、 mk的帮助信息
							1 一般情况下载bsp中使用 ./mk --help 

					2）mk文件分析
						1、 
							1、 shell脚本分为：变量定义、函数、代码。shell脚本程序的结构非常类似于C语言程序。
							2、 shell程序和C语言程序很大的一个差异就是shell没有main函数，shell脚本执行时也是先执行主函数的，不过主函数没有放在一个类似于main这样的函数中，而是直接放在全局下的一些代码。
							3、 shell程序执行时首先执行变量定义，然后执行主函数，其他函数在主函数代码中被调用执行。

						2、 主函数
							1、 shell 脚本实现控制编译。实现原理：用一个函数来编译一个（譬如编译内核用 build_kernel 函数，编译inand的uboot就用 build_bootloader_uboot_inand 函数），然后用一些相应的一些变量来控制这个函数要不要被编译。
								shift 可以让shell脚本参数 左移，即： $2 --> $1

						3、 buildroot引入
							1、 之前自己从零开始构建跟文件系统，一路下来事情还挺多，步骤比较麻烦。
							2、 较差编译工具链 arm-linux-gcc ，目前是直接从官方拿过来直接使用，官方的工具链从何而来？实际上交叉编译工具链都是有gcc配置编译生成的，这个配置编译过程比较复杂，一般人自己去配置编译得到自己交叉编译工具链是比较麻烦的，所以经常都是用别人做好的。
							3、 buildroot就是一个集成包，这个包里继承了交叉编译工具链的制作，以及整个rootfs 的配置编译过程。也就是说，使用 buildroot 可以很简便的得到一个做好的文件夹形式的跟文件系统。
							4、 buildroot见很多的东西集成进来后，移植了 Linux kernel的make xxx_defconfig + make menuconfig 的2步配置法， 我们可以再buildroot的配置界面下完成集成在里边的所有东西的配置，然后直接 make 就可以最终得到文件夹形式的 rootfs 。 
							5、 make x210ii_defconfig 
								1、 先make xxx_defconfig 
								2、 make menuconfig
								3、 make

							6、 make及其错误解决
								1、 直接make会遇到很多错误，这些错误原因都是因为ubuntu中缺乏一些必要的软件包造成的，解决办法就是下载这些软件包。
								2、 apt-cache search xxx 在源中查找关键字 xxx。
							
							7、 编译后的结果查看与分析
								1、 编译生成的文件夹格式的 rootfs 在 buildroot/output/images/rootfs.tar 。我们赋值到了根目录下的release目录下去，这个文件就是一个完整的可以工作的文件架形式的rootfs。


