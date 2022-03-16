/*
 * @Author: your name
 * @Date: 2021-07-04 15:35:49
 * @LastEditTime: 2021-07-04 16:57:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \pwm_rtc\rtc.h
 */

#ifndef __RTC_H_
#define __RTC_H_

#define RTC_BASE    0xE2800000
#define rINTP       (*((volatile unsigned int *)(RTC_BASE + 0x30)))    
#define rRTCCON     (*((volatile unsigned int *)(RTC_BASE + 0x40)))
#define rTICCNT     (*((volatile unsigned int *)(RTC_BASE + 0x44)))
#define rRTCALM     (*((volatile unsigned int *)(RTC_BASE + 0x50)))
#define rALMSEC     (*((volatile unsigned int *)(RTC_BASE + 0x54)))
#define rALMMIN     (*((volatile unsigned int *)(RTC_BASE + 0x58)))
#define rALMHOUR    (*((volatile unsigned int *)(RTC_BASE + 0x5C)))
#define rALMDAY     (*((volatile unsigned int *)(RTC_BASE + 0x60)))
#define rALMMON     (*((volatile unsigned int *)(RTC_BASE + 0x64)))
#define rALMYEAR    (*((volatile unsigned int *)(RTC_BASE + 0x68)))
#define rBCDSEC     (*((volatile unsigned int *)(RTC_BASE + 0x70)))
#define rBCDMIN     (*((volatile unsigned int *)(RTC_BASE + 0x74)))
#define rBCDHOUR    (*((volatile unsigned int *)(RTC_BASE + 0x78)))
#define rBCDDAYWEEK (*((volatile unsigned int *)(RTC_BASE + 0x7C)))
#define rBCDDAY     (*((volatile unsigned int *)(RTC_BASE + 0x80)))
#define rBCDMON     (*((volatile unsigned int *)(RTC_BASE + 0x84)))
#define rBCDYEAR    (*((volatile unsigned int *)(RTC_BASE + 0x88)))
#define rCURTICCNT  (*((volatile unsigned int *)(RTC_BASE + 0x90)))

struct rtc_time {
    unsigned int year;
    unsigned int month;
    unsigned int date;      // 日
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int day;       // 星期几
};

void rtc_settime(const struct rtc_time *p);
void rtc_gettime(struct rtc_time *p);

void rtc_setalarm(unsigned int times);
void isr_rtc_alarm(void);

#endif // !__RTC_H_
#define __RTC_H_


