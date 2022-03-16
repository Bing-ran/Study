// led register
#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244

#define rGPJ0CON *((volatile unsigned int *)GPJ0CON)
#define rGPJ0DAT *((volatile unsigned int *)GPJ0DAT)

void delay(void);

// 该函数要实现led闪烁效果
void led_blink(void)
{
	// led初始化，也就是把GPJ0CON中设置为输出模式
	int flag = 3;

	rGPJ0CON = 0x11111111;

	while (1)
	{
		// led亮
		rGPJ0DAT = (0<<3) | (0<<4) | (0<<5);
		// 延时
		delay();
		// led灭
		rGPJ0DAT = (1<<3) | (1<<4) | (1<<5);
		// 延时
		delay();
		flag++;
		if(flag > 5){
			flag = 3;
		}
	}
}


void delay(void)
{
	volatile unsigned int i = 90000;		// volatile 让编译器不要优化，这样才能真正的减
	while (i--);							// 才能消耗时间，实现delay
}



#if 0
	// 第一步：吧 0x11111111 写入 0xE0200240 位置。
	ldr r0, =0x11111111			// 前两句表示 ldr 是一个伪指令： = 号确定的。 # 表示是一个指令，因为需要编译器来判断这个数是合法立即数还是非法立即数。
	ldr r1, =GPJ0CON
	str r0, [r1]				// 寄存器间接寻址。 把 r0 中的数写入到 r1 中的数位地址的内存中去

loop:
	// 第二部：向data寄存器写
	ldr r0, =((0<<3) | (1 << 4) | (0<<5))
	ldr r1, =GPJ0DAT
	str r0, [r1]

	// 延时
	bl delay

	// 第二部：向data寄存器写
	ldr r0, =((1<<3) | (0 << 4) | (1<<5))
	ldr r1, =GPJ0DAT
	str r0, [r1]

	bl delay

	b loop


// 延时函数
delay:
	ldr r2, =10000000
	ldr r3, =0x0 

delay_loop:
	sub r2, r2, #1		// r2 -=1;
	cmp r2, r3			// cmp会影响z标志位，如果r2 == r3 则 z = 1，下一句eq就会成立
	bne delay_loop
	mov pc, lr 
#endif




