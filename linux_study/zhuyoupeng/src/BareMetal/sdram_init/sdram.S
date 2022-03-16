/**
*	文件名：led.S
*	作者：Master_r
*	描述：汇编代码点亮led
*/


// watchdog register
#define WTCON 0xE2700000

// stack
#define SVC_STACK 0xD0037D80

.global _start					// 把 _start 的连接属性改为外部，这样其他文件就可以看见 _start了

_start:
	// 关看门狗
	ldr r0, =0x0 
	ldr r1, =WTCON
	str r0, [r1]

	// 设置栈
	ldr sp, =SVC_STACK


	// 开/关iCache
	mrc p15, 0, r0, c1, c0,0;		// 取出cp15的c1到r0中
	//bic r0, r0, #(1<<12)			// bit12 置0 关icache
	orr r0, r0, #(1<<12)			// bit12 置1 开icache
	mcr p15, 0, r0, c1, c0,0;

	// 重定位
	// adr 指令用于加载 _start当前运行地址
	adr r0, _start
	// ldr 指令用于加载 _start的链接地址：0xd0024000
	ldr r1, =_start
	// bss段的起始地址
	ldr r2, =bss_start	// 就是我们重定位代码的结束地址，重定位只需重定位嗲码段和数据段即可
	cmp r0, r1			// 比较 _start 的运行地址和链接地址是否相等。
	beq clean_bss		// 如果相等说明不需要重定位，所以跳过 copy_loop ，直接clean_bss
						// 如果不相等说明需要重定位，那么直接执行下面的copy_loop进行重定位
						// 重定位完成后继续执行 clean_bss

// 用汇编实现循环
copy_loop:
	ldr r3, [r0], #4	// 源
	str r3, [r1], #4	// 目的  这两句代码就完成了4字节内容的拷贝。
	cmp r1, r2			// r1 和 r2 都是 ldr 加载的，都是链接地址，所以 r1 不断+4 总能相等。
	bne copy_loop		// 不相等则执行

	// 清bss段，其实就是在链接地址处把 bss 段全部清零
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1			// 如果r0等于r1，说明bss段为空，直接下去
	beq run_on_dram		// 清除 bss 完之后的地址
	mov r2, #0			// r2 = 0
clear_loop:
	str r2, [r0], #4	// 先将 r2 中的值放入 r0 所指向的内存地址（r0中的值作为内存地址），然后 r0 = r0 + 4
	cmp r0, r1
	bne clear_loop


run_on_dram:
	// 长跳转led_blink开始第二阶段
	ldr pc, =led_blink		// ldr 指令实现长跳转

	// 这里可以使用C程序， bl调用c程序
	// 汇编这个死循环不能丢
	b .



