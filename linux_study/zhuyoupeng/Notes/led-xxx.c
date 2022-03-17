
struct led_classdev *my_led;

static void __init xxx_led_init(void){
	
	/*  用户insmod安装驱动模块时调用该函数
		该函数的主要任务就是去使用led驱动框架提供的
	    设备注册函数来注册一个设备
	*/
	int ret;
	
	my_led.name = "my_led";
	my_led.brightness_set =  
	
	ret = led_classdev_register(NULL,&led->cdev);
	if(ret < 0)
	{
		printk(KERN_ERROR "led_classdev_register failed!\n");
		return ret;
	}
}

static void __init xxx_led_exit(void){
	
}

module_init(xxx_led_init);
module_exit(xxx_led_exit);

MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("S3C24XX LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s3c24xx_led");
