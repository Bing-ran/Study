/*
 * @Author: your name
 * @Date: 2021-07-21 22:51:05
 * @LastEditTime: 2021-08-05 21:40:50
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
#include <linux/gpio.h>

#define GPIO_LED1	S5PV210_GPJ0(3)
#define GPIO_LED2	S5PV210_GPJ0(4)
#define GPIO_LED3	S5PV210_GPJ0(5)

#define X210_led_ON 	(0)
#define X210_led_OFF 	(1)

struct led_classdev my_led_cdev1;
struct led_classdev my_led_cdev2;

int major = 0;

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
		gpio_set_value(GPIO_LED1, X210_led_OFF);
    } else {
		// 用户给了个非0，希望led亮， 
	   gpio_set_value(GPIO_LED1, X210_led_ON);
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
		gpio_set_value(GPIO_LED2, X210_led_OFF);
    } else {
		// 用户给了个非0，希望led亮， 
	   gpio_set_value(GPIO_LED2, X210_led_ON);
    }
	
}

struct gpio gpio_array[] = {
	{GPIO_LED1,		0,		"led1_gpj0.3"},
	{GPIO_LED2,		0,		"led1_gpj0.4"},
	{GPIO_LED3,		0,		"led1_gpj0.5"},
};

static const struct file_operations led_drv = {
	.owner		= THIS_MODULE,
};

// 模块安装函数
static int __init chrdev_init(void)
{	
	int ret = -1;
	printk(KERN_INFO "chrdev_init helloworld init\n");

	//  register  char dev
	major = register_chrdev(0, "LED1", &led_drv);
	if (major < 0) {
		printk("register_chrdev failed\n");
		goto fail1;
	}

	printk(KERN_INFO "major : %d", major);
	// 申请驱动用到的资源，当前驱动中就是gpio的资源
	if(gpio_request_array(gpio_array, sizeof(gpio_array)/sizeof(gpio_array[0]))) {
		printk(KERN_ERR "Failed ro request GPIO_%d\n", GPIO_LED1);
		return -EBUSY;
	} else {
		gpio_direction_output(GPIO_LED1, 1);
		gpio_direction_output(GPIO_LED2, 1);
		gpio_direction_output(GPIO_LED3, 1);
	}

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
	return 0;

flag1:

fail1:
	unregister_chrdev(major, "LED1");
	return -1;
}

// 模块下载函数
static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit helloworld exit\n");
	
	led_classdev_unregister(&my_led_cdev2);
	led_classdev_unregister(&my_led_cdev1);
	gpio_free_array(gpio_array, sizeof(gpio_array)/sizeof(gpio_array[0]));
	unregister_chrdev(major, "LED1");
}


module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("Master_r <935338030@qq.com>");				// 描述模块的作者
MODULE_DESCRIPTION("s5pv210 led driver");	// 描述模块的介绍信息
MODULE_ALIAS("s5pv210");			// 描述模块的别名信息








