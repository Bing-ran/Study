/*
 * @Author       : Master_r
 * @Date         : 2021-06-28 23:28:50
 * @LastEditTime : 2021-07-03 17:09:40
 * @LastEditors  : Master_r
 * @Description  : 中断初始化相关操作
 * @FilePath     : \key_irq\int.c
 */
#include "int.h"
#include "stdio.h"


void reset_exception(void)
{
	printf("reset_exception.\n");
}

void undef_exception(void)
{
	printf("undef_exception.\n");
}

void sotf_int_exception(void)
{
	printf("sotf_int_exception.\n");
}

void prefetch_exception(void)
{
	printf("prefetch_exception.\n");
}

void data_exception(void)
{
	printf("data_exception.\n");
}

// 处理流程 ： 异常向量表与处理函数绑定 --> 禁止所有中断 --> 清除VIC0ADDR寄存器（硬件会将中断处理函数的地址自动存储到这里）
// --> 选择中断类型

// 主要功能：绑定第一阶段异常向量表；禁止所有中断；选择所有中断类型为IRQ；
// 清除VICnADDR为0
/**
 * @function     : system_init_exception
 * @description  : 绑定中断向量表，调用中断控制器初始化函数
 * @param         {*}
 * @return        {*}
 * @note         : 
 */
void system_init_exception(void)
{
	// 第一阶段处理，绑定异常向量表
	r_exception_reset = (unsigned int)reset_exception;
	r_exception_undef = (unsigned int)undef_exception;
	r_exception_sotf_int = (unsigned int)sotf_int_exception;
	r_exception_prefetch = (unsigned int)prefetch_exception;
	r_exception_data = (unsigned int)data_exception;
	r_exception_irq = (unsigned int)IRQ_handle;
	r_exception_fiq = (unsigned int)IRQ_handle;
	
	// 初始化中断控制器的基本寄存器
	intc_init();
}

// 清除需要处理的中断的中断处理函数的地址
/**
 * @function: 
 * @description: 
 * @param  {*}
 * @return {*}
 * @nots : 
 */
void intc_clearvectaddr(void)
{
    // VICxADDR:当前正在处理的中断的中断处理函数的地址
    VIC0ADDR = 0;
    VIC1ADDR = 0;
    VIC2ADDR = 0;
    VIC3ADDR = 0;
}

// 初始化中断控制器
void intc_init(void)
{
    // 禁止所有中断
	// 为什么在中断初始化之初要禁止所有中断？
	// 因为中断一旦打开，因为外部或者硬件自己的原因产生中断后一定就会寻找isr
	// 而我们可能认为自己用不到这个中断就没有提供isr，这时它自动拿到的就是乱码
	// 则程序很可能跑飞，所以不用的中断一定要关掉。
	// 一般的做法是先全部关掉，然后再逐一打开自己感兴趣的中断。一旦打开就必须
	// 给这个中断提供相应的isr并绑定好。
    VIC0INTENCLEAR = 0xffffffff;
    VIC1INTENCLEAR = 0xffffffff;
    VIC2INTENCLEAR = 0xffffffff;
    VIC3INTENCLEAR = 0xffffffff;

    // 选择中断类型为IRQ
    VIC0INTSELECT = 0x0;
    VIC1INTSELECT = 0x0;
    VIC2INTSELECT = 0x0;
    VIC3INTSELECT = 0x0;

    // 清VICxADDR
    intc_clearvectaddr();
}


// 绑定我们写的isr到VICnVECTADDR寄存器
// 绑定过之后我们就把isr地址交给硬件了，剩下的我们不用管了，硬件自己会处理
// 等发生相应中断的时候，我们直接到相应的VICnADDR中去取isr地址即可。
// 参数：intnum是int.h定义的物理中断号，handler是函数指针，就是我们写的isr

// VIC0VECTADDR定义为VIC0VECTADDR0寄存器的地址，就相当于是VIC0VECTADDR0～31这个
// 数组（这个数组就是一个函数指针数组）的首地址，然后具体计算每一个中断的时候
// 只需要首地址+偏移量即可。
/*******************************************************************
 * 函数名：intc_setvectaddr
 * 函数作用：将制定中断号的isr（中断处理函数）绑定至该中断的VICECTADDR寄存器
 * param:   unsigned long intnum    - 中断号（int.h中的宏定义）
 *          void (*handler)(void)   - 中断处理函数
 * 返回值： 无
 * 注意：无
 * *****************************************************************/
/**
 * @function     : intc_setvectaddr
 * @description  : 绑定我们写的isr（中断函数）
 * @param         {unsigned long}   - intnum（与int.h中的宏定义对应）
 * @param         {void}            - (*handler)(void) isr
 * @return       ：
 * @note         : 
 */
void intc_setvectaddr(unsigned long intnum, void (*handler)(void))
{
    int rem_n = intnum % 32;
    int vic_n = intnum / 32;

    switch(vic_n) {
        case VIC0INT:
            *( (volatile unsigned long *)(VIC0VECTADDR + 4 * rem_n) ) = (unsigned)handler;
            break;
        case VIC1INT:
            *( (volatile unsigned long *)(VIC1VECTADDR + 4 * rem_n) ) = (unsigned)handler;
            break;
        case VIC2INT:
            *( (volatile unsigned long *)(VIC2VECTADDR + 4 * rem_n) ) = (unsigned)handler;
            break;
        case VIC3INT:
            *( (volatile unsigned long *)(VIC3VECTADDR + 4 * rem_n) ) = (unsigned)handler;
            break;
        default:
            break;
    }

    return;
}


// 使能中断
// 通过传参的intnum来使能某个具体的中断源，终端号在int.h中定义，十五里终端号
/**
 * @function     : intc_enable
 * @description  : 使能对应中断号
 * @param         {unsigned long} intnum 中断号
 * @return        {*}
 * @note         : 
 */
void intc_enable(unsigned long intnum)
{
    unsigned long temp;
	// 确定intnum在哪个寄存器的哪一位
	// <32就是0～31，必然在VIC0
    int vic_n = intnum / 32;
    int rem_n = intnum % 32;

    switch (vic_n){
        case VIC0INT:
            temp = VIC0INTENABLE;
            temp |= (1<<rem_n);
            VIC0INTENABLE = temp;
            break;
        case VIC1INT:
            temp = VIC1INTENABLE;
            temp |= (1<<rem_n);
            VIC1INTENABLE = temp;
            break;
        case VIC2INT:
            temp = VIC2INTENABLE;
            temp |= (1<<rem_n);
            VIC2INTENABLE = temp;
            break;
        case VIC3INT:
            temp = VIC3INTENABLE;
            temp |= (1<<rem_n);
            VIC3INTENABLE = temp;
            break;
        default:    // 默认全部打开
            VIC0INTENABLE = 0xFFFFFFFF;
            VIC1INTENABLE = 0xFFFFFFFF;
            VIC2INTENABLE = 0xFFFFFFFF;
            VIC3INTENABLE = 0xFFFFFFFF;
            break;
    }
}

// 禁止中断
// 通过传参的intnum来禁止某个具体的中断源，中断号在int.h中定义，是物理中断号
void intc_disable(unsigned long intnum)
{
    unsigned long temp;
    int vic_n = intnum / 32;
    int rem_n = intnum % 32;

    switch (vic_n){
        case VIC0INT:
            temp = VIC0INTENCLEAR;
            temp |= (1<<rem_n);
            VIC0INTENCLEAR = temp;
            break;
        case VIC1INT:
            temp = VIC1INTENCLEAR;
            temp |= (1<<rem_n);
            VIC1INTENCLEAR = temp;
            break;
        case VIC2INT:
            temp = VIC2INTENCLEAR;
            temp |= (1<<rem_n);
            VIC2INTENCLEAR = temp;
            break;
        case VIC3INT:
            temp = VIC3INTENCLEAR;
            temp |= (1<<rem_n);
            VIC3INTENCLEAR = temp;
            break;
        default:
            VIC0INTENCLEAR = 0xFFFFFFFF;
            VIC1INTENCLEAR = 0xFFFFFFFF;
            VIC2INTENCLEAR = 0xFFFFFFFF;
            VIC3INTENCLEAR = 0xFFFFFFFF;
            break;
    }
}


// 通过读取VICnIRQSTATUS寄存器，判断其中哪个有一位为1，来得知哪个VIC发生中断了
unsigned long intc_getvicirqstatus(unsigned long ucontroller)
{
    unsigned long int temp = 0;
    switch(ucontroller){
        case VIC0INT:
            temp = VIC0IRQSTATUS;
            break;
        case VIC1INT:
            temp = VIC1IRQSTATUS;
            break;
        case VIC2INT:
            temp = VIC2IRQSTATUS;
            break;
        case VIC3INT:
            temp = VIC3IRQSTATUS;
            break;
        default:
            break;
    }

    return temp;
}


// 真正的中断处理程序。意思就是说这里只考虑中断处理，不考虑保护/恢复现场
void irq_handler(void)
{
	printf("irq_handler.\n");
	// SoC支持很多个（在低端CPU例如2440中有30多个，在210中有100多个）中断
	// 这么多中断irq在第一个阶段走的是一条路，都会进入到irq_handler来
	// 我们在irq_handler中要去区分究竟是哪个中断发生了，然后再去调用该中断
	// 对应的isr。
	
	
	// 虽然硬件已经自动帮我们把isr放入了VICnADDR中，但是因为有4个，所以我们必须
	// 先去软件的检查出来到底哪个VIC中断了，也就是说isr到底在哪个VICADDR寄存器中
	unsigned long vicaddr[4] = {VIC0ADDR,VIC1ADDR,VIC2ADDR,VIC3ADDR};
    int i=0;
    void (*isr)(void) = NULL;

    for(i=0; i<4; i++)
    {
		// 发生一个中断时，4个VIC中有3个是全0，1个的其中一位不是0
        if(intc_getvicirqstatus(i) != 0)
        {
            isr = (void (*)(void)) vicaddr[i];
            break;
        }
    }
    (*isr)();		// 通过函数指针来调用函数
	//isr();			// 正确的，和上面的一个效果
	//*isr();		// 错误
}







