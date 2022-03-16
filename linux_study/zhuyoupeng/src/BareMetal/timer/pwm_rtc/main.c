/*
 * @Author       : Master_r
 * @Date         : 2021-06-28 23:28:25
 * @LastEditTime: 2021-07-04 17:29:19
 * @LastEditors: Please set LastEditors
 * @Description  : main主函数
 * @FilePath     : \key_irq\main.c
 */
#include "main.h"

int main(void)
{
	volatile unsigned int i = 0, j = 0, temp = 0;
	struct rtc_time new_rtc = {
		.year = 2021,
		.month = 7,
		.date = 5,
		.hour = 16,
		.minute = 25,
		.second = 30,
		.day = 190,
	};
	
	struct rtc_time old_rtc = {0};

	uart_init();

	// 如果程序要是用中断，就要调用中断初始化来初始化
	printf("-------------- rtc timer ----------------------\n");
/*

	rtc_gettime(&old_rtc);
	printf("the old_time : %d-%d-%d %d:%d:%d\n", old_rtc.year, old_rtc.month, old_rtc.date, old_rtc.hour, old_rtc.minute, old_rtc.second);
	
	

	rtc_gettime(&old_rtc);
	printf("the new_time : %d-%d-%d %d:%d:%d\n", old_rtc.year, old_rtc.month, old_rtc.date, old_rtc.hour, old_rtc.minute, old_rtc.second);
*/
	rtc_settime(&new_rtc);
	
	system_init_exception();
	rtc_setalarm(30);
	intc_setvectaddr(NUM_RTC_ALARM, isr_rtc_alarm);
	intc_enable(NUM_RTC_ALARM);

	while (1) {
		rtc_gettime(&old_rtc);
		printf("\t---the new_time : %d-%d-%d %d:%d:%d ---\t", old_rtc.year, old_rtc.month, old_rtc.date, old_rtc.hour, old_rtc.minute, old_rtc.second);
	
		for (i=0; i<10000; i++)
			for (j=0; j<1000; j++);
	}

	return temp;
}
