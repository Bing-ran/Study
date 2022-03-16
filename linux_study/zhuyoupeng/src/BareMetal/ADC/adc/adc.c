/*
 * @Author: your name
 * @Date: 2021-07-13 21:07:50
 * @LastEditTime: 2021-07-13 22:38:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \adc\adc.c
 */
#include "adc.h"

#define ADC_BASE    0xE1700000
#define rTSADCCON0   (*((volatile unsigned int *)(ADC_BASE + 0x00)))
#define rTSCON0      (*((volatile unsigned int *)(ADC_BASE + 0x04)))
#define rTSDATX0     (*((volatile unsigned int *)(ADC_BASE + 0x0C)))
#define rTSDATY0     (*((volatile unsigned int *)(ADC_BASE + 0x10)))
#define rCLRINTADC0  (*((volatile unsigned int *)(ADC_BASE + 0x18)))
#define rADCMUX      (*((volatile unsigned int *)(ADC_BASE + 0x1C)))

 /**
  * @description: 初始化adc控制器的函数
  * @param {*}
  * @return {*}
  */
 static void adc_init(void)
 {
     // 第一步：
    rTSADCCON0 |= (1<<16);  // 12位 ADC output resolution selection
    rTSADCCON0 |= (1<<14);  // A/D converter prescaler enable
    rTSADCCON0 &= ~(0XFF<<6);
    rTSADCCON0 |= (65<<6);  // convertor clock = 66 / 66 M=1MHz, MSPS=20KHz

    rTSADCCON0 &= ~(1<<2);  // normal operation mode
    rTSADCCON0 &= ~(1<<1);  // disable start by read mode

    rADCMUX &= ~(0x0f<<0);  // MUX选择ADCINT0
 }

static void delay_my(void){
    volatile unsigned int i,j;
    for(i=0;i<2000;i++){ 
        for(j=0;j<2000;j++);
    }
}

/**
 * @description: adc测试函数
 * @param {*}
 * @return {*}
 */
void adc_test(void)
{
    int val = 0;
    adc_init();

    while (1)
    {  
        // 第一步：开启ADC转化
        rTSADCCON0 |= (1<<0);
        // 第二部：等待ADC转换完成
        while(!(rTSADCCON0 & (1<<15)));
        // 第三部：读取ADC的数字值
        val = rTSDATX0;
        printf("normal X : %d\t", (val & (1<<14)));
        printf("val X : %d\n", (val & (0xfff<<0)));

        val = rTSDATY0;
        printf("normal Y : %d\t", (val & (1<<14)));
        printf("val Y : %d\n", (val & (0xfff<<0)));
        // 第四部：处理/显示数字值

        // 第五步：延时一段时间
        delay_my();
        
    }
}