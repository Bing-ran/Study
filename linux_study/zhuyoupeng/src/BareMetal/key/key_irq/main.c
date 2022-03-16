/*
 * @Author       : Master_r
 * @Date         : 2021-06-28 23:28:25
 * @LastEditTime: 2021-07-04 14:46:56
 * @LastEditors: Please set LastEditors
 * @Description  : main主函数
 * @FilePath     : \key_irq\main.c
 */
#include "stdio.h"
#include "int.h"
#include "key.h"

void uart_init(void);

#define KEY_INTNUM2			NUM_EINT2			// left
#define KEY_INTNUM3			NUM_EINT3			// down
#define KEY_INTNUM16_19		NUM_EINT16_31		// 其余4个共用

static delay_my(void)
{
	int num = 9999999;
	while(num--);
}


int main(void)
{
	uart_init();
	printf("---------------- init intterupt ----------------------\n");
	key_init_intterupt();

	// 如果程序要是用中断，就要调用中断初始化来初始化
	system_init_exception();
	
	// 绑定isr到中断控制器
	intc_setvectaddr(KEY_INTNUM2, isr_eint2);
	intc_setvectaddr(KEY_INTNUM3, isr_eint3);
	intc_setvectaddr(KEY_INTNUM16_19, isr_eint16171819);

	// 使能中断
	intc_enable(KEY_INTNUM2);
	intc_enable(KEY_INTNUM3);
	intc_enable(KEY_INTNUM16_19);

	while (1)
	{
		delay_my();
	}
	

	return 0;
}
