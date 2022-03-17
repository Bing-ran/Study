#include <linux/module.h>		// module_init  module_exit
#include <linux/init.h>			// __init   __exit
#include <linux/fs.h>
#include <linux/leds.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-bank.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <mach/gpio.h>


#define GPIO_LED1	S5PV210_GPJ0(3)
#define GPIO_LED2	S5PV210_GPJ0(4)
#define GPIO_LED3	S5PV210_GPJ0(5)

#define X210_LED_OFF	1			// X210中LED是正极接电源，负极节GPIO
#define X210_LED_ON		0			// 所以1是灭，0是亮


struct s5PV210_gpio_led {
	struct led_classdev		 cdev;
	struct s5PV210_led_platdata	*pdata;
};

static inline struct s5PV210_gpio_led *pdev_to_gpio(struct platform_device *dev)
{
	return platform_get_drvdata(dev);
}

static inline struct s5PV210_gpio_led *to_gpio(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct s3c24xx_gpio_led, cdev);
}

// 这个函数就是要去完成具体的硬件读写任务的
static void s5pv210_led1_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	struct s5PV210_gpio_led *led = to_gpio(led_cdev);
	struct s5PV210_led_platdata *pd = led->pdata;
	
	printk(KERN_INFO "s5pv210_led1_set\n");
	
	//writel(0x11111111, GPJ0CON);
	
	// 在这里根据用户设置的值来操作硬件
	// 用户设置的值就是value
	if (value == LED_OFF)
	{
		// 用户给了个0，希望LED灭
		//writel(0x11111111, GPJ0CON);
		// 读改写三部曲
		//writel((readl(GPJ0DAT) | (1<<3)), GPJ0DAT);
//		gpio_set_value(led->pdata->gpio, X210_LED_OFF);
		gpio_set_value(pd->gpio, X210_LED_OFF);
	}
	else
	{
		// 用户给的是非0，希望LED亮
		//writel(0x11111111, GPJ0CON);
		//writel((readl(GPJ0DAT) & ~(1<<3)), GPJ0DAT);
//		gpio_set_value(led->pdata->gpio, X210_LED_ON);
		gpio_set_value(pd->gpio, X210_LED_ON);
	}
}

static int s5pv210_led_probe(struct platform_device *dev)
{
	// 该函数的主要任务就是去使用led驱动框架提供的设备注册函数来注册一个设备
	int ret = -1;
	
	struct s5PV210_led_platdata *pdata = dev->dev.platform_data;
	struct s5PV210_gpio_led *p;
	
	led = kzalloc(sizeof(struct s5PV210_gpio_led), GFP_KERNEL);
	if (led == NULL) {
		dev_err(&dev->dev, "No memory for device\n");
		return -ENOMEM;
	}
	
	platform_set_drvdata(dev, led);
	
	// 在这里去申请驱动用到的各种资源，当前驱动中就是GPIO资源
	if (gpio_request(pdata->gpio, pdata->name)) 
	{
		printk(KERN_ERR "gpio_request failed\n");
	} 
	else 
	{
		// 设置为输出模式，并且默认输出1让LED灯灭
		gpio_direction_output(pdata->gpio, 1);
	}

	// led1
	p->cdev.name = pdata->name;
	p->cdev.brightness = 0;	
	p->cdev.brightness_set = s5pv210_led1_set;
	
	ret = led_classdev_register(&dev->dev, &p->cdev);
	if (ret < 0) {
		printk(KERN_ERR "led_classdev_register failed\n");
		return ret;
	}
	
	return 0;
}

static int s5pv210_led_remove(struct platform_device *dev)
{
	struct s5PV210_gpio_led *p = pdev_to_gpio(dev);
	
	led_classdev_unregister(&p->cdev);
	gpio_free(p->pdata->name);
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
	
	return 0;
}

static void __exit s5pv210_led_exit(void)
{
	platform_driver_unregister(&s5pv210_led_driver);
}

module_init(s5pv210_led_init);
module_exit(s5pv210_led_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");							// 描述模块的许可证
MODULE_AUTHOR("aston <1264671872@qq.com>");		// 描述模块的作者
MODULE_DESCRIPTION("s5pv210 led driver");		// 描述模块的介绍信息
MODULE_ALIAS("s5pv210_led");					// 描述模块的别名信息








