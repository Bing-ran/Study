/*
 * @Author       : Master_r
 * @Date         : 2021-07-03 22:45:07
 * @LastEditTime : 2021-07-03 23:23:45
 * @LastEditors  : Master_r
 * @Description  : main主函数
 * @FilePath     : \pwm_buzzer\main.c
 */
#include "stdio.h"

void uart_init(void);

int main(void)
{
	int i = 0, flag = 0;
	uart_init();
	timer2_pwm_init();

	while(i >= 0){
		//uart_getchar();
		//uart_putchar('a' + i);
		printf("%s：%c\n", "hello", 'c' + i);
		delay();
	
		if ((flag == 0) && (++i == 'z' - 'a'))
		{
			flag = 1;
		}
		else if ((flag == 1) && (--i == 0))
		{
			flag = 0;
		}
		
	}

	return 0;
}