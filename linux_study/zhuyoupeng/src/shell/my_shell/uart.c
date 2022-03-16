/*
 * @Author: your name
 * @Date: 2021-07-18 21:25:40
 * @LastEditTime: 2021-07-18 21:57:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \my_shell\uart.c
 */

#define GPA0CON 		0xE0200000
#define ULCON0			0xE2900000
#define UCON0 			0xE2900004
#define UMCON0			0xE290000C
#define UFCON0			0xE2900008
#define UBRDIV0			0xE2900028
#define UDIVSLOT0 		0xE290002C

#define UTRSTAT0		0xE2900010
#define UTXH0			0xE2900020
#define URXH0			0xE2900024

#define rGPA0CON 		(*(volatile unsigned int *)GPA0CON)
#define rULCON0 		(*(volatile unsigned int *)ULCON0)
#define rUCON0 			(*(volatile unsigned int *)UCON0)
#define rUMCON0 		(*(volatile unsigned int *)UMCON0)
#define rUFCON0 		(*(volatile unsigned int *)UFCON0)
#define rUBRDIV0 		(*(volatile unsigned int *)UBRDIV0)
#define rUDIVSLOT0 		(*(volatile unsigned int *)UDIVSLOT0)

#define rUTRSTAT0 		(*(volatile unsigned int *)UTRSTAT0)
#define rUTXH0 			(*(volatile unsigned int *)UTXH0)
#define rURXH0 			(*(volatile unsigned int *)URXH0)

// 串口初始化
void uart_init(void)
{
	// GPA0 初始化
	rGPA0CON &= ~(0xff<<0); 			// 把bit 0:7 全部清零
	rGPA0CON |= 0x00000022;			// 复用gpio

	// UCON寄存器
	rULCON0 = 0x3;

	// UCON寄存器
	rUCON0 = 0x5;

	// UMCON0寄存器
	rUMCON0 = 0;

	// UFCON0寄存器
	rUFCON0 = 0;

	// 波特率计算 DIV_VAL = (PCLK / (bps x 16)) −1
	//rUBRDIV0 = 34;						// PCLK_PSYS 66  	余数 0.8
	//rUDIVSLOT0 = 0xDFDD;				// 0.8 * 16

	rUBRDIV0 = 35;						// PCLK_PSYS 66.7	余数 0.18
	rUDIVSLOT0 = 0x0888;				// 0.18 * 16
}

// 串口发送1个字节
void uart_putchar(char c)
{
	while(!(rUTRSTAT0 & (1<<1)));
	rUTXH0 = c;
}

void putchar(char c)
{
	if(c == '\n') {
		uart_putchar('\r');
	}
	uart_putchar(c);
}

void puts(const char *str)
{
	while(*str != '\0') {
		putchar(*str);
		str++;
	}
}

// 串口接收1个字节
char uart_getchar(void)
{
	while(!(rUTRSTAT0 & (1<<0)));
	return rURXH0;
}

char getchar()
{
	char c;
	c = uart_getchar();
	if(c == '\r') {
		return '\n';
	}
	return c;
}

char *gets(char *p)
{
	char *p1 = p;
	char ch;

	while(getchar() != '\n') {	// 用户输入的不是推个键
		// 回显
		if(ch != '\b') {
			putchar(ch);
			*p++ = ch;
		} else {	// 用户输入的是退格键
			// \b 只会让secureCRT终端输出指针向后退一格，但是那个要删掉的字符还在，删掉的方法就是下面3行
			if(p > p1) {
				putchar('\b');	
				putchar(' ');
				putchar('\b');		// 3行处退格键回显
				p--;				// 光标退一格，指针指向了要删除的那个格子
				*p = '\0';			// 填充'\0'以替换要删除的那个字符				
			}
		}
	}

	*p = '\0';
	putchar('\n');

	return p1;
}

void memset(char *buf, char c, int len)
{
	int i = 0;

	for (i = 0; i < len; i++) {
		buf[i] = c;
	}
}