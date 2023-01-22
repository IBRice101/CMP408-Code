#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>

// This kernel module will register an interrupt request (IRQ) on the falling edge 
// (i.e., button press) of the specified GPIO pin. This will then trigger the speedtest.py program

// FIXME: run only once and then wait

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Isaac Basque-Rice");
MODULE_DESCRIPTION("A simple Linux kernel module to detect button press on a Raspberry Pi");

// Ensure button is always on this GPIO pin, if it's not change this value
#define BUTTON_GPIO 17

#define BUTTON_MAJOR 64

// pin number / interrupt controller to which BUTTON_GPIO is mapped
unsigned int irq_number;

// Variables for speedtester registration
#define REGISTER_SAPP _IO('R', 'g')
static struct task_struct *task = NULL;

// Define for signal sending
#define SIGNR 44

// This calls an interrupt service routine when the interrupt is triggered
static irq_handler_t button_signal_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    struct siginfo info;
    printk("Interrupt was triggered and ISR was called\n");

	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task) < 0)
			printk("Error sending signal\n");
	}
	return (irq_handler_t) IRQ_HANDLED;
}

// This is an IOCTL function for registering the speedtester to the LKM
static long int button_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
    if(cmd == REGISTER_SAPP) {
        task = get_current();
        printk("Userspace app with PID %d is registered \n", task->pid);
    }
    return 0;
}

// This function is called when the device file is opened
static int button_close(struct inode *device_file, struct file *instance) {
	if(task != NULL)
		task = NULL;
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.release = button_close,
	.unlocked_ioctl = button_ioctl,
};


static int __init button_init(void)
{
    int err = 0;

	printk("Loading module... ");

    // Request the GPIO
    if (!gpio_is_valid(BUTTON_GPIO)) {
        printk(KERN_ERR "Invalid GPIO\n");
        return -ENODEV;
    }

	if(gpio_direction_input(BUTTON_GPIO)) {
		printk("Error!\nCan not set GPIO 17 to input!\n");
		gpio_free(BUTTON_GPIO);
		return -1;
	}

    if ((err = gpio_request(BUTTON_GPIO, "button_gpio"))) {
        printk(KERN_ERR "Failed to request GPIO\n");
        return err;
    }

	gpio_set_debounce(BUTTON_GPIO, 300);

    // set up interrupt
    irq_number = gpio_to_irq(BUTTON_GPIO);

    free_irq(irq_number, NULL);

    // Enable IRQ on falling edge (button press)
    if ((err = request_irq(irq_number, (irq_handler_t) button_signal_handler, IRQF_TRIGGER_FALLING, "button_irq", NULL))) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpio_free(BUTTON_GPIO);
        return err;
    }

	if(register_chrdev(BUTTON_MAJOR, "gpio_irq_signal", &fops) < 0) 
    {
		printk("Error!\n Can't register device Number!\n");
		gpio_free(BUTTON_GPIO);
		free_irq(irq_number, NULL);
	}

    printk(KERN_INFO "Button module initialized\n");
	printk("GPIO pin is mapped to IRQ no.: %d\n", irq_number);

    return 0;
}

static void __exit button_exit(void)
{
    printk("Unloading module...");
    // Free the IRQ and GPIO
    free_irq(irq_number, NULL);
    gpio_free(BUTTON_GPIO);
	unregister_chrdev(BUTTON_MAJOR, "gpio_irq_signal");
    printk("Module unloaded\n");
}

module_init(button_init);
module_exit(button_exit);

