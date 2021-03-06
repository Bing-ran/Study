一、内存管理之栈（stack）

1、什么是栈
	一种数据结构，C语言中使用栈来保存局部变量。栈是管理内存的。

2、栈管理内存的特点（小内存、自动化）
	栈顶、栈底（该指针不变）。
	栈  ： 先进后出 first in last out;
	队列： 先进先出：first in first out  -->  FIFO

3、栈的应用举例：局部变量
	C语言中的 局部变量 是用栈来实现的。
	我们在C语言中定义一个局部变量时（int a）,编译器会在栈中分配一段空间（4字节）给这个局部变量用（分配时栈顶指针会移动给出空间，给局部变量a用的意思就是：这4字节的栈内存的内存地址和变量名绑定）。对应栈的操作是入栈。

	注意：这里栈的指针的移动和内存分配是自动的（栈自己完成，不用我们写代码去操作）

	然后等函数退出时，局部变量要灭亡。对应栈的操作是弹栈（出栈）。出栈时也是栈顶指针移动将栈空间中与a关联的空间释放（自动完成）

	优点：方便，分配和回收都不用程序员关心，系统自动完成。
	
	分析：C语言中，定义局部变量时如果未初始化，则值是随机的，为什么?
		定义局部变量，其实就是在栈中通过移动栈指针来给程序提供一个内存空间和这个局部变量绑定。因为这段内存空间在栈上，而栈内存是反复使用的，所以未初始化的局部变量其值是随机的。（栈内存容易 脏）

4、栈的约束（预定栈大小不灵活，怕溢出）
	栈有大小，其大小不好设置。太小容易溢出，太大怕浪费。
	栈溢出危害很大，一定要避免。

	C语言定义局部变量不能太大或太多。
	使用递归解决问题时，一定要注意递归收敛。