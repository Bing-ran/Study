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

#define GPH0CON	0xE0200C00
#define GPH0DAT	0xE0200C04

#define GPH2CON	0xE0200C40
#define GPH2DAT	0xE0200C44

#define rGPH0CON	(*(volatile unsigned int *)GPH0CON)
#define rGPH0DAT	(*(volatile unsigned int *)GPH0DAT)
#define rGPH2CON	(*(volatile unsigned int *)GPH2CON)
#define rGPH2DAT	(*(volatile unsigned int *)GPH2DAT)

#define EXT_INT_0_CON	0xE0200E00
#define EXT_INT_2_CON	0xE0200E08
#define EXT_INT_0_PEND	0xE0200F40
#define EXT_INT_2_PEND	0xE0200F48
#define EXT_INT_0_MAKD	0xE0200F00
#define EXT_INT_2_MAKD	0xE0200F08

#define rEXT_INT_0_CON		(*(volatile unsigned int *)EXT_INT_0_CON)
#define rEXT_INT_2_CON		(*(volatile unsigned int *)EXT_INT_2_CON)
#define rEXT_INT_0_PEND		(*(volatile unsigned int *)EXT_INT_0_PEND)
#define rEXT_INT_2_PEND		(*(volatile unsigned int *)EXT_INT_2_PEND)
#define rEXT_INT_0_MASK		(*(volatile unsigned int *)EXT_INT_0_MAKD)
#define rEXT_INT_2_MASK		(*(volatile unsigned int *)EXT_INT_2_MAKD)

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