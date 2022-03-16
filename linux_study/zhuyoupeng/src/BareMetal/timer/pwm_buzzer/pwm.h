/*** 
 * @Author       : Master_r
 * @Date         : 2021-07-03 22:52:16
 * @LastEditTime : 2021-07-03 23:23:06
 * @LastEditors  : Master_r
 * @Description  : pwm.c头文件
 * @FilePath     : \pwm_buzzer\pwm.h
 */

#ifndef __PWM_H_
#define __PWM_H_

#define GPD0CON     (0xE02000A0)
#define rGPD0CON    (*(volatile unsigned int *)GPD0CON)

#define TCFG0   (0xE2500000)
#define TCFG1   (0xE2500004)
#define TCON    (0xE2500008)
#define TCNTB2  (0xE2500024)
#define TCMPB2  (0xE2500028)

#define rTCFG0  (*(volatile unsigned int *)TCFG0)
#define rTCFG1  (*(volatile unsigned int *)TCFG1)
#define rTCON   (*(volatile unsigned int *)TCON)
#define rTCNTB2 (*(volatile unsigned int *)TCNTB2)
#define rTCMPB2 (*(volatile unsigned int *)TCMPB2)

void timer2_pwm_init(void);

#endif // !__PWM__H

