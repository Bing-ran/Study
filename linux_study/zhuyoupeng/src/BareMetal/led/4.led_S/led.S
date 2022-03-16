/**
*	文件名：led.S
*	作者：Master_r
*	描述：汇编代码点亮led
*/
#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244

#define OPEN 1

.global _start					// 把 _start 的连接属性改为外部，这样其他文件就可以看见 _start了

_start:
	// 第一步：吧 0x11111111 写入 0xE0200240 位置。

	ldr r0, =0x11111111			// 前两句表示 ldr 是一个伪指令： = 号确定的。 # 表示是一个指令，因为需要编译器来判断这个数是合法立即数还是非法立即数。
	ldr r1, =GPJ0CON
	str r0, [r1]				// 寄存器间接寻址。 把 r0 中的数写入到 r1 中的数位地址的内存中去

	ldr r4, =3
	ldr r5, =3

loop:
	// 第二部：向data寄存器写
	cmp r4, r5
	beq open3

	cmp r4, r6
	beq open4

	cmp r4, r7
	beq open5


open3:
	ldr r4, =4
	ldr r0, =~(OPEN << 3)
	b continue

open4:
	ldr r0, =~(OPEN << 4)
	b continue

open5:
	ldr r4, =3
	ldr r0, =~(OPEN << 5)
	b continue

continue:	
	ldr r1, =GPJ0DAT
	str r0, [r1]

	bl delay
	sub r4, r4, #1

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



