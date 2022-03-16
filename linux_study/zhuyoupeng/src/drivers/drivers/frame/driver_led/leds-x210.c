/*
 * @Author: your name
 * @Date: 2021-07-21 22:51:05
 * @LastEditTime: 2021-08-02 22:34:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \character_driver\module_test.c
 */
#include <linux/module.h>		// module_init  module_exit
#include <linux/init.h>			// __init   __exit
#include <linux/fs.h>	// 
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <mach/gpio-bank.h>
#include <mach/regs-gpio.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/leds.h>

#define GPJ0CON 	0xe0200240

#define LED_NAME "leds"
typedef struct {
	volatile unsigned int con;
	volatile unsigned int data;
}pGPJ0_t;
pGPJ0_t *gpj0_s;
struct led_classdev my_led_cdev1;
struct led_classdev my_led_cdev2;

// 完成store方法的具体操作
void s5pv210_led_set1(struct led_classdev *led_cdev,
					  enum led_brightness brightness)
{
	printk(KERN_INFO "x210_led_set\n");

	// 在这里根据用户设置的值来操作硬件
	// 用户设置的值就是value

	if (brightness == LED_OFF)
	{
		// 用户给了个0，希望led灭
		// 读改写
        writel(readl(&gpj0_s->data) | (1<<3), &gpj0_s->data);
    } else {
		// 用户给了个非0，希望led亮， 
		//gpj0_s->data = (0<<3) | (0<<4) | (0<<5);
	   writel(readl(&gpj0_s->data) & (~(1<<3)), &gpj0_s->data);
    }
	
}

void s5pv210_led_set2(struct led_classdev *led_cdev,
					  enum led_brightness brightness)
{
	printk(KERN_INFO "x210_led_set\n");

	// 在这里根据用户设置的值来操作硬件
	// 用户设置的值就是value

	if (brightness == LED_OFF)
	{
		// 用户给了个0，希望led灭
		// 读改写
        writel(readl(&gpj0_s->data) | (1<<4), &gpj0_s->data);
    } else {
		// 用户给了个非0，希望led亮， 
		//gpj0_s->data = (0<<3) | (0<<4) | (0<<5);
	   writel(readl(&gpj0_s->data) & (~(1<<4)), &gpj0_s->data);
    }
	
}

// 模块安装函数
static int __init chrdev_init(void)
{	
	int ret = -1;
	printk(KERN_INFO "chrdev_init helloworld init\n");
	// 用户insmod安装驱动模块时会调用该函数
	// 该函数的主要功能就是去使用led驱动框架提供的设备注册函数来注册一个设备
	my_led_cdev1.name = "my_leds1";
	my_led_cdev1.brightness = 0;
	my_led_cdev1.max_brightness = 128;
	my_led_cdev1.brightness_set = s5pv210_led_set1;

	ret = led_classdev_register(NULL, &my_led_cdev1);
	if (ret < 0) {
		printk(KERN_ERR "led_classdev_register failed\n");
		goto flag1;
	}

	my_led_cdev2.name = "my_leds2";
	my_led_cdev2.brightness = 0;
	my_led_cdev2.max_brightness = 128;
	my_led_cdev2.brightness_set = s5pv210_led_set2;

	ret = led_classdev_register(NULL, &my_led_cdev2);
	if (ret < 0) {
		printk(KERN_ERR "led_classdev_register failed\n");
		goto flag1;
	}

	// 动态映射操作led
	if (!request_mem_region(GPJ0CON, sizeof(gpj0_s), LED_NAME))
		goto region;

	gpj0_s = ioremap(GPJ0CON, sizeof(gpj0_s));
	if (!gpj0_s)
		goto release_intmem;


	gpj0_s->con = 0x11111111;
	writel(((1<<3) | (1<<4) | (1<<5)), &gpj0_s->data);
	return 0;

release_intmem:
	release_mem_region(GPJ0CON, sizeof(gpj0_s));
region:
	led_classdev_unregister(&my_led_cdev1);
flag1:

	return -1;
}

// 模块下载函数
static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit helloworld exit\n");
	led_classdev_unregister(&my_led_cdev1);
	led_classdev_unregister(&my_led_cdev2);
	iounmap(gpj0_s);
	release_mem_region(GPJ0CON, sizeof(gpj0_s));

}


module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("Master_r <935338030@qq.com>");				// 描述模块的作者
MODULE_DESCRIPTION("s5pv210 led driver");	// 描述模块的介绍信息
MODULE_ALIAS("s5pv210");			// 描述模块的别名信息








