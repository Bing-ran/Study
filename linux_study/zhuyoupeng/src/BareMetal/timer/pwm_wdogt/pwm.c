/*
 * @Author       : Master_r
 * @Date         : 2021-07-03 22:51:46
 * @LastEditTime: 2021-07-04 00:27:40
 * @LastEditors: Please set LastEditors
 * @Description  : ""
 * @FilePath     : \pwm_buzzer\pwm.c
 */

#include "pwm.h"

/**
 * @function     : timer2_pwm_init
 * @description  : 初始化wdt，定时10s
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void timer2_wdt_init(void)
{
    // 设置prescaler = 65,产生1MHz信号 即：1us
    rWTCON &= ~(0xff << 8);
    rWTCON |= (65 << 8);

    // MUX = 1/128,产生4KHz信号 128us
    rWTCON &= ~(3 << 3);
    rWTCON |= (3 << 3);

    // 产生中断信号
    rWTCON |= (1 << 2);
    // 禁止复位信号
    rWTCON &= ~(1 << 0);

    // MUX
    rWTDAT |= 3 * 1000 * 8; // 10s
    rWTCNT  |= 3 * 1000 * 8; // 10s

    rWTCON |= (1 << 5);     // 开启wdt

}