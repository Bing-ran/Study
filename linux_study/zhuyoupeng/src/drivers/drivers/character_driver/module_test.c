/*
 * @Author: your name
 * @Date: 2021-07-21 22:51:05
 * @LastEditTime: 2021-07-21 23:01:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \character_driver\module_test.c
 */
#include <linux/module.h>		// module_init  module_exit
#include <linux/init.h>			// __init   __exit
#include <linux/fs.h>	// 
#include <linux/kernel.h>
#include <linux/uaccess.h>

#define MY_MAJOR	200
#define AUTO_MAJOT	0
#define MY_NAME		"mytest_driver"

int major_ret = -1;

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

// 自定义一个file_operations结构体变量，并填充
static const struct file_operations mytest_fops = {
	.owner		= THIS_MODULE,			// 惯例，都这么写
	.read		= mytest_chrdev_read,
	.write		= mytest_chrdev_write,
	.open		= mytest_chrdev_open,
	.release	= mytest_chrdev_release,		// 对应 close 函数
};

// 模块安装函数
static int __init chrdev_init(void)
{	

	printk(KERN_INFO "chrdev_init helloworld init\n");
	//printk("<7>" "chrdev_init helloworld init\n");
	//printk("<7> chrdev_init helloworld init\n");

	// 在mocule_init宏调用的函数中去注册字符设备驱动
	major_ret = register_chrdev(AUTO_MAJOT, MY_NAME, &mytest_fops);
	if(major_ret < 0) {
		printk(KERN_ERR "register_chrdev faield\n");
		return -EINVAL;
	}

	printk(KERN_INFO "register_chrdev success... major : %d\n", major_ret);

	return 0;
}

// 模块下载函数
static void __exit chrdev_exit(void)
{
	printk(KERN_INFO "chrdev_exit helloworld exit\n");

	// 在module_exit宏调用的函数中去注销字符设备驱动
	unregister_chrdev(major_ret, MY_NAME);
}


module_init(chrdev_init);
module_exit(chrdev_exit);

// MODULE_xxx这种宏作用是用来添加模块描述信息
MODULE_LICENSE("GPL");				// 描述模块的许可证
MODULE_AUTHOR("aston");				// 描述模块的作者
MODULE_DESCRIPTION("module test");	// 描述模块的介绍信息
MODULE_ALIAS("alias xxx");			// 描述模块的别名信息








