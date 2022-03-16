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

#define WTCON       0xE2700000
#define WTDAT       0xE2700004
#define WTCNT       0xE2700008
#define WTCLRINT    0xE270000C

#define rWTCON      (*(volatile unsigned int *)WTCON)
#define rWTDAT      (*(volatile unsigned int *)WTDAT)
#define rWTCNT      (*(volatile unsigned int *)WTCNT)
#define rWTCLRINT   (*(volatile unsigned int *)WTCLRINT)


#endif // !__PWM__H

