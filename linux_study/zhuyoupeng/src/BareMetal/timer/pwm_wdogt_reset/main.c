/*
 * @Author       : Master_r
 * @Date         : 2021-06-28 23:28:25
 * @LastEditTime: 2021-07-04 14:56:04
 * @LastEditors: Please set LastEditors
 * @Description  : main主函数
 * @FilePath     : \key_irq\main.c
 */
#include "stdio.h"
#include "int.h"
#include "wdt.h"

void uart_init(void);

#define WDT_INTNUM			NUM_WDT			// 看门狗

static delay_my(void)
{
	int num = 9999999;
	while(num--);
}


int main(void)
{
	uart_init();
	timer2_wdt_init();

	// 如果程序要是用中断，就要调用中断初始化来初始化
	printf("-------------- pwm timer wdt ----------------------\n");

	while (1);

	return 0;
}
