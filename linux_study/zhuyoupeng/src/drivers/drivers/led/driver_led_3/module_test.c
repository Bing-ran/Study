/*
 * @Author: your name
 * @Date: 2021-07-21 22:51:05
 * @LastEditTime: 2021-08-01 17:14:07
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

#define MY_MAJOR	200
#define AUTO_MAJOT	0
#define MY_NAME		"mytest_driver"
#define MY_NODE		"testaaaaaaaa"

#define MYCON	3

dev_t mydev;
static struct class *my_class;


typedef struct GPJ0REG {
	volatile unsigned int gpj0_con;
	volatile unsigned int gpj0_data;
}gpj0_reg_t;

#define GPJ0_REGBASE 		0xe0200240
gpj0_reg_t *pGPJ0_REG;

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
	pGPJ0_REG->gpj0_con = 0x11111111;	// 初始化
	if (cmd == 1)
	{
		pGPJ0_REG->gpj0_data = (0<<3) | (0<<4) | (0<<5);
	} else {
		pGPJ0_REG->gpj0_data = (1<<3) | (1<<4) | (1<<5);
	}
	
	return 0;	
}
static struct cdev my_device_cdev;

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
	// 在mocule_init宏调用的函数中去注册字符设备驱动

#if 0  // 旧的方法注册字符设备
	// 注册字符设备驱动
	major_ret = register_chrdev(AUTO_MAJOT, MY_NAME, &mytest_fops);
	if(major_ret < 0) {
		printk(KERN_ERR "register_chrdev faield\n");
		return -EINVAL;
	}
#else // 新的方法注册字符设备
	// 第一步：注册/分配主次设备号
/*	mydev = MKDEV(MY_MAJOR,0);  // 
	if (register_chrdev_region(mydev, 3, "LED")) {
		printk(KERN_ERR "CUSE: failed to register chrdev region\n");
		return -1;
	}
*/
	// 第一步：分配主次设备号
	result = alloc_chrdev_region(&mydev, 0, MYCON, "LED");
	if (result < 0) {
		printk(KERN_ERR "alloc_chrdev_region() failed for bsr\n");
		goto error_cdev;
	}

	printk(KERN_INFO "CUSE: success to register chrdev region, major:[%d],minor:[%d]\n", MAJOR(mydev), MINOR(mydev));
	// 第二步：注册字符设备驱动
	cdev_init(&my_device_cdev, &mytest_fops);	//  cdev绑定ops

	if(cdev_add(&my_device_cdev, mydev, MYCON)) {
		printk(KERN_ERR "Unable to get my_device major %d\n", mydev);
		goto error_cdev;
	}


	my_class = class_create(THIS_MODULE, MY_NAME);
	if (IS_ERR(my_class)) {
		goto error_cdev;
	}

	device_create(my_class, NULL, mydev, NULL, MY_NODE);

#endif

	// 动态映射方式操作LED
	if (!request_mem_region(GPJ0_REGBASE, sizeof(gpj0_reg_t), "GPJ0")) {
		goto err_free_mem;
	}

	pGPJ0_REG = ioremap(GPJ0_REGBASE, sizeof(gpj0_reg_t));
	if (pGPJ0_REG ==  NULL) {
		goto err_map;
	}

	printk(KERN_INFO "register_chrdev success...\n");

	writel(0x11111111, &pGPJ0_REG->gpj0_con);
	writel(((0<<3) | (0<<4) | (0<<5)), &pGPJ0_REG->gpj0_data);

	return 0;
err_map:
	release_mem_region(GPJ0_REGBASE, sizeof(gpj0_reg_t));
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
	cdev_del(&my_device_cdev);

	// 第二部：注销申请的主次设备号
	unregister_chrdev_region(mydev, MYCON);
#endif

	writel(((1<<3) | (1<<4) | (1<<5)), &pGPJ0_REG->gpj0_data);
	iounmap(pGPJ0_REG);
	release_mem_region(GPJ0_REGBASE, sizeof(gpj0_reg_t));
	
	device_destroy(my_class,  mydev);
	class_destroy(my_class);
}


module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("Master_r");				// 描述模块的作者
MODULE_DESCRIPTION("module test");	// 描述模块的介绍信息
MODULE_ALIAS("alias xxx");			// 描述模块的别名信息








