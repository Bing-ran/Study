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

run_on_dram:

	// 长跳转led_blink开始第二阶段
	ldr pc, =main		// ldr 指令实现长跳转

	// 这里可以使用C程序， bl调用c程序
	// 汇编这个死循环不能丢
	b .



