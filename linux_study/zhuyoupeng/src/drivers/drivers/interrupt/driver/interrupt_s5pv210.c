#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <plat/gpio-cfg.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/gpio.h>
#include <linux/irqflags.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

/*
 * X210:
 *
 * POWER  -> EINT1   -> GPH0_1
 * LEFT   -> EINT2   -> GPH0_2
 * DOWN   -> EINT3   -> GPH0_3
 * UP     -> KP_COL0 -> GPH2_0
 * RIGHT  -> KP_COL1 -> GPH2_1
 * MENU   -> KP_COL3 -> GPH2_3 (KEY_A)
 * BACK   -> KP_COL2 -> GPH2_2 (KEY_B)
 */


#define BUTTON_IRQ  IRQ_EINT2
static struct input_dev *button_dev;

void tasklet_func(unsigned long param)
{
	int flag = 0;
	printk("tasklet_func:interrupt buton half \n");
	s3c_gpio_cfgpin(S5PV210_GPH0(2), S3C_GPIO_SFN(0x0));		// input模式
	flag = gpio_get_value(S5PV210_GPH0(2));
	s3c_gpio_cfgpin(S5PV210_GPH0(2), S3C_GPIO_SFN(0x0f));		// eint2模式

	input_report_key(button_dev, KEY_LEFT, !flag);
	
	input_sync(button_dev);
}

void work_queue_func(struct work_struct *work)
{
	int flag = 0;
	printk("work_queue_func:interrupt buton half \n");
	s3c_gpio_cfgpin(S5PV210_GPH0(2), S3C_GPIO_SFN(0x0));		// input模式
	flag = gpio_get_value(S5PV210_GPH0(2));
	s3c_gpio_cfgpin(S5PV210_GPH0(2), S3C_GPIO_SFN(0x0f));		// eint2模式

	input_report_key(button_dev, KEY_LEFT, !flag);
	
	input_sync(button_dev);
}
DECLARE_TASKLET(my_tasklet, tasklet_func, 0);
DECLARE_WORK(my_workqueue, work_queue_func);

static irqreturn_t button_interrupt(int irq, void *dummy)
{
	printk("button_interrupt:interrupt top half \n");
	//tasklet_schedule(&my_tasklet);

	schedule_work(&my_workqueue);
	return IRQ_HANDLED;
}

static int __init button_init(void) 
{ 
	int error;

	error = gpio_request(S5PV210_GPH0(2), "GPH0_2");
	if(error)
		printk("button-x210: request gpio GPH0(2) fail");
	s3c_gpio_cfgpin(S5PV210_GPH0(2), S3C_GPIO_SFN(0x0f));		// eint2 模式

	if (request_irq(BUTTON_IRQ, button_interrupt, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "button-x210", NULL)) {
                printk(KERN_ERR "button.c: Can't allocate irq %d\n", BUTTON_IRQ);
                return -EBUSY;
        }

	button_dev = input_allocate_device();
	if (!button_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}

//	set_bit(EV_KEY, button_dev->evbit);
//	set_bit(KEY_LEFT, button_dev->keybit);

	button_dev->evbit[0] = BIT_MASK(EV_KEY);
	button_dev->keybit[BIT_WORD(KEY_LEFT)] = BIT_MASK(KEY_LEFT);

	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	}

	return 0;

 err_free_dev:
	input_free_device(button_dev);
 err_free_irq:
 	gpio_free(S5PV210_GPH0(2));
	free_irq(BUTTON_IRQ, button_interrupt);
	return error; 
}

static void __exit button_exit(void)
{
    input_unregister_device(button_dev);
	free_irq(BUTTON_IRQ, button_interrupt);
	gpio_free(S5PV210_GPH0(2));
}

module_init(button_init); 
module_exit(button_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aston <1264671872@qq.com>");
MODULE_DESCRIPTION("key driver for x210 button.");
