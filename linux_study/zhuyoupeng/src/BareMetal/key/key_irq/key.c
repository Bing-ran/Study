#include "stdio.h"
#include "led.h"

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

//----------------------------轮训方式来处理----------------------
void key_init(void)
{
	// 设置GPHxCON寄存器，设置输入模式,bit8~15全部设置为0，即可
	rGPH0CON &= ~(0xFF<<8);

	// 设置GPHxCON寄存器，设置输入模式,bit0~15全部设置为0，即可
	rGPH2CON &= ~(0xFFFF<<0);
}

void delay20ms(void)
{
	// 这个函数作用是延时20ms
	// 因为我们这里是裸机程序，且重点不是真的要消抖，而是教学
	// 所以我这里这个程序只是象征性的，并没有实体
	// 如果是研发，那就要花时间真的调试出延时20ms的程序
	int i, j;
	
	for (i=0; i<100; i++)
	{
		for (j=0; j<1000; j++)
		{
			i * j;
		}
	}
}

void key_polling(void)
{
	// 依次，挨个去读出每个GPIO的值，判断其值为1还是0.如果为1则按键按下，为0则弹起
	
	// 轮询的意思就是反复循环判断有无按键，隔很短时间
	while (1)
	{
		// 对应开发板上标着LEFT的那个按键
		if (rGPH0DAT & (1<<2))
		{
			// 为1，说明没有按键
			led_off();
		}
		else
		{
			// 添加消抖
			// 第一步，延时
			delay20ms();
			// 第二步，再次检验按键状态
			if (!(rGPH0DAT & (1<<2)))
			{
				// 为0，说明有按键
				led1();
				printf("key left.\n");
			}
		}
		
		// 对应开发板上标着DOWN的那个按键
		if (rGPH0DAT & (1<<3))
		{
			// 为1，说明没有按键
			led_off();
		}
		else
		{
			// 为0，说明有按键
			led2();
			printf("key down.\n");
		}
		
		// 对应开发板上标着UP的那个按键
		if (rGPH2DAT & (1<<0))
		{
			// 为1，说明没有按键
			led_off();
		}
		else
		{
			// 为0，说明有按键
			led3();
		}
	}
}


//-------------------------外部中断方式处理------------------------
/**
 * @function     : key_init_intterupt
 * @description  : 中断方式处理按键初始化程序
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void key_init_intterupt(void)
{
	// 将GPH0_2 GPH0_3设置为外部中断模式
	rGPH0CON = (0xff << 8);

	// 将GPH2_0 GPH0_1 GPH0_2 GPH0_3设置为外部中断模式
	rGPH2CON = (0xffff << 0);

	// 中断触发模式设置
	rEXT_INT_0_CON &= ~(0xff << 8);		// 先清零
	rEXT_INT_0_CON |= (2 << 8) | (2 << 12); // 设置EXTINT2 EXTINT3 下降沿触发

	rEXT_INT_2_CON &= ~(0xffff << 8);		// 先清零
	rEXT_INT_2_CON |= (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12); // 设置EXTINT2 EXTINT3 下降沿触发

	// 中断的允许
	rEXT_INT_0_MASK &= ~(3 << 2);
	rEXT_INT_2_MASK &= ~(0x0f << 0);

	// 清挂起
	rEXT_INT_0_PEND |= (3 << 2);
	rEXT_INT_2_PEND |= (0x0F << 0);
}

/**
 * @function     : isr_eint2
 * @description  : EINT2 isr
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void isr_eint2(void)
{
	// 中断干活代码
	printf("isr eint2 LEFT.\n");
	// 清PEND
	rEXT_INT_0_PEND |= (1 << 2);
	intc_clearvectaddr();
}

/**
 * @function     : isr_eint3
 * @description  : 
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void isr_eint3(void)
{
	// 中断干活代码
	printf("isr eint2 DOWN.\n");
	// 清PEND
	rEXT_INT_0_PEND |= (1 << 3);
	intc_clearvectaddr();
}

/**
 * @function     : isr_eint16171819
 * @description  : 
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void isr_eint16171819(void)
{
	// 中断干活代码，因为 16 - 31 共享中断，所以在此还需要再次去区分是哪个子中断
	if(rEXT_INT_2_PEND & (1<<0)) {
		printf("isr eint16.\n");
	}
	if(rEXT_INT_2_PEND & (1<<1)) {
		printf("isr eint17.\n");
	}
	if (rEXT_INT_2_PEND & (1<<2)) {
		printf("isr eint18.\n");
	}
	if (rEXT_INT_2_PEND & (1<<3))
	{
		printf("isr eint19.\n");
	}
	
	// 清PEND
	rEXT_INT_2_PEND |= (0x0f << 0);
	intc_clearvectaddr();
}