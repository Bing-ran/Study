/*
 * @Author       : Master_r
 * @Date         : 2021-07-03 22:51:46
 * @LastEditTime : 2021-07-03 23:25:34
 * @LastEditors  : Master_r
 * @Description  : ""
 * @FilePath     : \pwm_buzzer\pwm.c
 */

#include "pwm.h"

/**
 * @function     : timer2_pwm_init
 * @description  : 初始化timer2，输出pwm波形，频率2KHz、duty：50%
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void timer2_pwm_init(void)
{
    // 设置GPD0_2复用为pwm
    rGPD0CON &= ~(0xf << 8);
    rGPD0CON |= (2 << 8);

    // 设置pwm定时器的寄存器
    rTCFG0 &= ~(0xff << 8);
    rTCFG0 |= (65 << 8);    // prescaler = 65,预分频之后为1MHz

    rTCFG1 &= ~(0xf << 8);
    rTCFG1 |= (1 << 8);     // MUX = 1,即2分频 500KHz --> 2us
    // 时钟周期设置好，我们的时钟频率是500KHz，对应的时钟周期是2us。也就是说每隔2us
    // 记一次数。如果要定的时间是x，则对应TCNTB中应该写入 x/2us


    rTCON |= (1 << 15);     // 使能auto-reload，反复定时才能发出PWM波形

    rTCNTB2 = 250;          // 0.5ms / 2us
    rTCMPB2 = 20;          // duty = 20%
    
    // 第一次需要手工将TCNTB中的值刷新到TCNT中去
    rTCON |= (1 << 13);     // 刷新自动刷新功能
    rTCON &= ~(1 << 13);    // 关闭自动刷新功能

    rTCON |= (1 << 12);     // 开timer2定时器，需要把其他的设置好
}