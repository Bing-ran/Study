/*
 * @Author: your name
 * @Date: 2021-07-04 00:04:21
 * @LastEditTime: 2021-07-04 14:50:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \pwm_wdogt\wdt.c
 */

#include "wdt.h"

/**
 * @description: 中断服务函数
 * @param {*}
 * @return {*}
 */
void isr_wdt(void)
{
    static int i = 0;
    // 看门狗中断实际操作
    printf("wdt interrupt, i = %d\n", i);
    i++;

	rWTCLRINT = 1;
	intc_clearvectaddr();
}