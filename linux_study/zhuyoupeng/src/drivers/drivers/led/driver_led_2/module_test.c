/*
 * @Author: your name
 * @Date: 2021-07-21 22:51:05
 * @LastEditTime: 2021-07-30 00:38:15
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

#define MY_NAME		"mytest_driver"

static struct cdev *pdev = NULL;
struct class *my_class;

int major_ret = -1;
int mydev = 0;
unsigned int *GPJ0_VA;

#define MYCON	3
#define GPJ0CON_PA		0xe0200240

#define rGPJ0CON *((volatile unsigned int *)S5PV210_GPJ0CON)
#define rGPJ0DAT *((volatile unsigned int *)S5PV210_GPJ0DAT)

static int mytest_chrdev_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "mytest_chrdev_open\n");
	return 0;
}

static ssize_t mytest_chrdev_read(struct file *file, char __user *buf,
			   size_t count, loff_t *ppos)
{
	int ret = -1;

	printk(KERN_INFO "mytest_chrdev_read\n");
	ret = copy_to_user(buf, "xxx", sizeof("xxx") > count ? count : sizeof("xxx"));
	if (ret)
	{
		return -EFAULT;
	}
	
	return 0;
}

static ssize_t mytest_chrdev_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	int ret = -1;
	char buff[64] = {0};

	ret = copy_from_user(buff, buf, sizeof(buff));
	if (ret)
	{
		return -EFAULT;
	}
	
	printk(KERN_INFO "mytest_chrdev_write, info : %s\n", buff);
	return 0;
}

static int mytest_chrdev_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "mytest_chrdev_release\n");	
	return 0;
}

static int mytest_chrdev_ioctl(struct inode *inode, struct file *file,
	unsigned int cmd, unsigned long arg)
{
	rGPJ0CON = 0x11111111;	// 初始化
	if (cmd == 1)
	{
		rGPJ0DAT = (0<<3) | (0<<4) | (0<<5);
	} else {
		rGPJ0DAT = (1<<3) | (1<<4) | (1<<5);
	}
	
	
	return 0;	
}

// 自定义一个file_operations结构体变量，并填充
static const struct file_operations mytest_fops = {
	.owner		= THIS_MODULE,			// 惯例，都这么写
	.read		= mytest_chrdev_read,
	.write		= mytest_chrdev_write,
	.open		= mytest_chrdev_open,
	.release	= mytest_chrdev_release,		// 对应 close 函数
	.ioctl		= mytest_chrdev_ioctl,
};

// 模块安装函数
static int __init chrdev_init(void)
{	
	int result = -1;
	printk(KERN_INFO "chrdev_init helloworld init\n");
	//printk("<7>" "chrdev_init helloworld init\n");
	//printk("<7> chrdev_init helloworld init\n");

	// 在mocule_init宏调用的函数中去注册字符设备驱动

	// 第一步：分配主次设备号
	result = alloc_chrdev_region(&mydev, 0, MYCON, "LED");
	if (result < 0) {
		printk(KERN_ERR "alloc_chrdev_region() failed for bsr\n");
		goto error_cdev;
	}

	printk(KERN_INFO "CUSE: success to register chrdev region, major:[%d],minor:[%d]\n", MAJOR(mydev), MINOR(mydev));
	// 第二步：注册字符设备驱动
	pdev = cdev_alloc();	// 之神实例化（分配内存）
	cdev_init(pdev, &mytest_fops);	//  cdev绑定ops

	if(cdev_add(pdev, mydev, MYCON)) {
		printk(KERN_ERR "Unable to get my_device major %d\n", mydev);
		goto error_cdev;
	}

	// 注册字符设备驱动完成后，添加设备类的操作，以让内核帮我们发信息给udev，让udev自动创建和删除设备文件
	my_class = class_create(THIS_MODULE, MY_NAME);
	if (IS_ERR(my_class)) {
		goto class_create_err;
	}
	// 最后一个参数字符串，就是我们将来要在/dev目录下创建的设备文件的名字，所以我们这里的文件名是/dev/test
	device_create(my_class, NULL, mydev, NULL, "test111");
	
	// 动态映射方式操作LED
	if (!request_mem_region(GPJ0CON_PA, 8, "GPJ0")) {
		goto err_free_mem;
	}


	GPJ0_VA = ioremap(GPJ0CON_PA, 8);
	if (GPJ0_VA ==  NULL) {
		goto err_map;
	}

	printk(KERN_INFO "register_chrdev success... major : %d\n", major_ret);

	*GPJ0_VA = 0x11111111;
	*(GPJ0_VA + 1) = (0<<3) | (0<<4) | (0<<5);
	//rGPJ0CON = 0x11111111;	// 初始化

	//rGPJ0DAT = (0<<3) | (0<<4) | (0<<5);

	return 0;
	
class_create_err:
	class_destroy(my_class);
err_map:
	release_mem_region(GPJ0CON_PA, 8);
err_free_mem:
	
error_cdev:

	return 0;
}

// 模块下载函数
static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit helloworld exit\n");

	// 在module_exit宏调用的函数中去注销字符设备驱动
#if 0
	unregister_chrdev(major_ret, MY_NAME);
#else 
	// 第一步：真正的注销字符设备驱动 cdev_del
	cdev_del(pdev);

	// 第二部：注销申请的主次设备号
	unregister_chrdev_region(mydev, MYCON);
	device_destroy(my_class, mydev);
	class_destroy(my_class);
#endif
	//rGPJ0DAT = (1<<3) | (1<<4) | (1<<5);
	*(GPJ0_VA + 1) = (1<<3) | (1<<4) | (1<<5);
	iounmap(GPJ0_VA);
	release_mem_region(GPJ0CON_PA, 8);
	
}


module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("aston");				// 描述模块的作者
MODULE_DESCRIPTION("module test");	// 描述模块的介绍信息
MODULE_ALIAS("alias xxx");			// 描述模块的别名信息








