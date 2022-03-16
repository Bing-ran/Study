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
	//orr r0, r0, #(1<<12)			// bit12 置1 开icache
	mcr p15, 0, r0, c1, c0,0;

	// 这里可以使用C程序， bl调用c程序
	
	bl led_blink

	// 汇编这个死循环不能丢
	b .



