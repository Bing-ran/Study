
static int __init chrdev_init(void)
{
	// 第1步：分配主次设备号
	alloc_chrdev_region();	
	
	// 第2步：注册字符设备驱动
	cdev_alloc();	
	
	//cdev_init(pcdev, &test_fops);
	pcdev->owner = THIS_MODULE;
	pcdev->ops = &test_fops;
	
	cdev_add();
	
	
	// 注册字符设备驱动完成后，添加设备类的操作，以让内核帮我们发信息
	// 给udev，让udev自动创建和删除设备文件
	test_class = class_create(THIS_MODULE, "aston_class");
	if (IS_ERR(test_class))
		return -EINVAL;
	// 最后1个参数字符串，就是我们将来要在/dev目录下创建的设备文件的名字
	// 所以我们这里要的文件名是/dev/test
	device_create(test_class, NULL, mydev, NULL, "test111");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("aston");				// 描述模块的作者
MODULE_DESCRIPTION("module test");	// 描述模块的介绍信息
MODULE_ALIAS("alias xxx");			// 描述模块的别名信息