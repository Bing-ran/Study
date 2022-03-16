/*
 * @Author: your name
 * @Date: 2021-07-04 15:35:42
 * @LastEditTime: 2021-07-04 17:26:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \pwm_rtc\rtc.c
 */
#include "rtc.h"

static unsigned int num_to_bcd(unsigned int num);
static unsigned int bcd_to_num(unsigned int bcd);

/**
 * @description: 
 * @param {const struct rtc_time} *time
 * @return {*}
 */
void rtc_settime(const struct rtc_time *p)
{
    // 打开rtc的读写开关
    rRTCCON |= (1 << 0);

    // 写RTC时间寄存器
    rBCDYEAR    = num_to_bcd(p->year - 2000);
    rBCDMON     = num_to_bcd(p->month);
    rBCDDAY     = num_to_bcd(p->date);
    rBCDHOUR    = num_to_bcd(p->hour);
    rBCDMIN     = num_to_bcd(p->minute);
    rBCDSEC     = num_to_bcd(p->second);
    rBCDDAYWEEK = num_to_bcd(p->day);

    // 关闭rtc的读写开关
    rRTCCON &= ~(1 << 0);
}

void rtc_gettime(struct rtc_time *p)
{
     // 打开rtc的读写开关
    rRTCCON |= (1 << 0);

    // 写RTC时间寄存器
    p->year = bcd_to_num(rBCDYEAR) + 2000;
    p->month = bcd_to_num(rBCDMON);
    p->date = bcd_to_num(rBCDDAY);
    p->hour = bcd_to_num(rBCDHOUR);
    p->minute = bcd_to_num(rBCDMIN);
    p->second = bcd_to_num(rBCDSEC);
    p->day = bcd_to_num(rBCDDAYWEEK);

    // 关闭rtc的读写开关
    rRTCCON &= ~(1 << 0);
}


/**
 * @description: 将十进制数专为bcd码，例如：56 --> 0x56
 * @param {unsigned int} num
 * @return {*}
 */
static unsigned int num_to_bcd(unsigned int num)
{
    // 第一步，把数字拆分
    // num / 10 num % 10

    // 第二部，组合
    return (((num / 10)<<4) | (num % 10));
}

/**
 * @description: 将bcd码转为十进制
 * @param {unsigned int} num
 * @return {*}
 */
static unsigned int bcd_to_num(unsigned int bcd)
{
    // 第一步，把数字拆分
    // (bcd & 0xf0) >> 4  bcd & 0x0f

    // 第二部，组合
    return ((((bcd & 0xf0) >> 4) * 10) + (bcd & 0x0f));
}

void rtc_setalarm(unsigned int times)
{
    rALMSEC = num_to_bcd(times);
    rRTCALM |= (1 << 0);
    rRTCALM |= (1 << 6);
}
void isr_rtc_alarm(void)
{
    printf("isr_rtc_alarm \n");

    rINTP |= (1 << 1);
    intc_clearvectaddr();
}