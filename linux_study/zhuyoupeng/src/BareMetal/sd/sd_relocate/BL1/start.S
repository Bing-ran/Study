/**
*	文件名：led.S
*	作者：Master_r
*	描述：汇编代码点亮led
*/


// watchdog register
#define WTCON 0xE2700000

// stack
#define SVC_STACK 0xD0037D80

// 开发板置锁
#define PS_HOLD_CONTROL 0xE010E81C
#define PS_HOLD_VALUE 0x301
.global _start					// 把 _start 的连接属性改为外部，这样其他文件就可以看见 _start了

_start:
	// 关看门狗
	ldr r0, =0x0 
	ldr r1, =WTCON
	str r0, [r1]

	// 供电锁存
	ldr r0, =PS_HOLD_CONTROL	/* PS_HOLD_CONTROL register */
	ldr r1, [r0]
	//orr r1, r1, #0x300			/* PS_HOLD output high	若果使用 301 编译器不认*/
	ldr r2, =0x301				/* 这样写就可以 */
	orr r1, r1, r2
	//orr r1, r1, #0x01 
	str r1, [r0]

	// 设置栈
	ldr sp, =SVC_STACK

	// 开/关iCache
	mrc p15, 0, r0, c1, c0,0;		// 取出cp15的c1到r0中
	//bic r0, r0, #(1<<12)			// bit12 置0 关icache
	orr r0, r0, #(1<<12)			// bit12 置1 开icache
	mcr p15, 0, r0, c1, c0,0;

	// 初始化ddr
	bl sdram_asm_init

	// 重定位：从SD卡第45扇区开始，赋值32个扇区内容到DDR的0x23E00000
	bl copy_bl2_to_ddr

	b .



