register_chrdev_region(dev_t from, unsigned count, const char *name)
	from:设备号
	count：此设备号个数
	name：名字

void cdev_init(struct cdev *cdev, const struct file_operations *fops)


int cdev_add(struct cdev *p, dev_t dev, unsigned count)




cdev_del(struct cdev *p)


void unregister_chrdev_region(dev_t from, unsigned count)