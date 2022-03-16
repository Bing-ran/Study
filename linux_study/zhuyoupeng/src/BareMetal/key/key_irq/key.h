/*** 
 * @Author       : Master_r
 * @Date         : 2021-07-03 16:06:26
 * @LastEditTime : 2021-07-03 16:20:58
 * @LastEditors  : Master_r
 * @Description  : 中断处理函数
 * @FilePath     : \key_irq\key.h
 */

#ifndef __KEY_H_
#define __KEY_H_

// 轮训方式
void key_init(void);
void delay20ms(void);
void key_polling(void);

// 按键中断方式初始化函数
void key_init_intterupt(void);

// 中断处理函数
void isr_eint2(void);
void isr_eint3(void);
void isr_eint16171819(void);

#endif // !1