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
#include <linux/platform_device.h>
#include <mach/led-gpio.h>
#include <linux/slab.h>

#define X210_led_ON 	(0)
#define X210_led_OFF 	(1)

struct s5pv210_gpio_led {
	struct led_classdev		 cdev;
	struct s5pv210_led_platdata	*pdata;
};

static inline struct s5pv210_gpio_led *pdev_to_gpio(struct platform_device *dev)
{
	return platform_get_drvdata(dev);
}

static inline struct s5pv210_gpio_led *to_gpio(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct s5pv210_gpio_led, cdev);
}

// 完成store方法的具体操作
void s5pv210_led_set(struct led_classdev *led_cdev,
					  enum led_brightness brightness)
{
	struct s5pv210_gpio_led *p = to_gpio(led_cdev);

	printk(KERN_INFO "x210_led_set\n");

	// 在这里根据用户设置的值来操作硬件
	// 用户设置的值就是value

	if (brightness == LED_OFF)
	{
		// 用户给了个0，希望led灭
		// 读改写
		gpio_set_value(p->pdata->gpio, X210_led_OFF);
    } else {
		// 用户给了个非0，希望led亮， 
	   gpio_set_value(p->pdata->gpio, X210_led_ON);
    }
	
}

static const struct file_operations led_drv = {
	.owner		= THIS_MODULE,
};

// 模块安装函数
static int s5pv210_led_probe(struct platform_device *dev)
{	
	int ret = -1;

	struct s5pv210_led_platdata *pdata = dev->dev.platform_data;
	struct s5pv210_gpio_led *led;

	printk(KERN_INFO " ----- s5pv210_led_probe ------\n");
 
	led = kzalloc(sizeof(struct s5pv210_gpio_led), GFP_KERNEL);
	if (led == NULL) {
		printk(KERN_ERR  "No memory for device\n");
		goto flag1;
	}

	platform_set_drvdata(dev, led);

	// 申请驱动用到的资源，当前驱动中就是gpio的资源

	if (gpio_request(pdata->gpio, pdata->name))
	{
		printk(KERN_ERR "Failed ro request GPIO_%d\n", pdata->gpio);
		goto flag2;
	} else {
		gpio_direction_output(pdata->gpio, 1);
	}
	
	// 用户insmod安装驱动模块时会调用该函数
	// 该函数的主要功能就是去使用led驱动框架提供的设备注册函数来注册一个设备
	led->cdev.name = pdata->name;
	led->cdev.brightness = 0;
	led->cdev.brightness_set = s5pv210_led_set;

	led->pdata = pdata;

	ret = led_classdev_register(&dev->dev, &led->cdev);
	if (ret < 0) {
		printk(KERN_ERR "led_classdev_register failed\n");
		goto flag3;
	}

	return 0;

flag3:
	gpio_free(pdata->gpio);
flag2:
	kfree(led);
flag1:
	return -1;
}

// 模块下载函数
static int s5pv210_led_remove(struct platform_device *dev)
{
	struct s5pv210_gpio_led *led = pdev_to_gpio(dev);
	
	printk(KERN_INFO "s5pv210_led_remove helloworld exit\n");

	led_classdev_unregister(&led->cdev);
	gpio_free(led->pdata->gpio);

	kfree(led);

	return 0;
}

static struct platform_driver s5pv210_led_driver = {
	.probe		= s5pv210_led_probe,
	.remove		= s5pv210_led_remove,
	.driver		= {
		.name		= "s5pv210_led",
		.owner		= THIS_MODULE,
	},
};

static int __init s5pv210_led_init(void)
{
	return platform_driver_register(&s5pv210_led_driver);
}

static void __exit s5pv210_led_exit(void)
{
	platform_driver_unregister(&s5pv210_led_driver);
}

module_init(s5pv210_led_init);
module_exit(s5pv210_led_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("Master_r <935338030@qq.com>");				// 描述模块的作者
MODULE_DESCRIPTION("s5pv210 led driver");	// 描述模块的介绍信息
MODULE_ALIAS("s5pv210");			// 描述模块的别名信息








