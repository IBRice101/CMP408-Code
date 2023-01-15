#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

// This kernel module will register an interrupt request (IRQ) on the falling edge 
// (i.e., button press) of the specified GPIO pin. This will then trigger the speedtest.py program

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Isaac Basque-Rice");
MODULE_DESCRIPTION("A simple Linux kernel module to detect button press on a Raspberry Pi");

// Ensure button is always on this GPIO pin, if it's not change this value
#define BUTTON_GPIO 15

#define BUTTON_MAJOR 64

// pin number / interrupt controller to which BUTTON_GPIO is mapped
unsigned int irq_number;

static int button_pressed = 0;

// Variables for speedtester registration
#define REGISTER_SAPP _IO('R', 'g')
static struct task_struct *task = NULL;

// Define for signal sending
#define SIGNR 42

// This calls an interrupt service routine when the interrupt is triggered
static irq_handler_t button_signal_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    struct siginfo info;
    printk("gpio_irq_signal: Interrupt was triggered and ISR was called!\n");

	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
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

// This will be called when the button is pressed
static irqreturn_t button_isr(int irq, void *data)
{
    if(!button_pressed){
        printk(KERN_INFO "Button press detected\n");
        button_pressed = 1; // ensure button press only registered once
    }
    return IRQ_HANDLED;
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

    // Request the GPIO
    if (!gpio_is_valid(BUTTON_GPIO)) {
        printk(KERN_ERR "Invalid GPIO\n");
        return -ENODEV;
    }
    if ((err = gpio_request(BUTTON_GPIO, "button_gpio"))) {
        printk(KERN_ERR "Failed to request GPIO\n");
        return err;
    }

    // Set the GPIO as an input
    gpio_direction_input(BUTTON_GPIO);

    // set up interrupt
    irq_number = gpio_to_irq(BUTTON_GPIO);

    // Enable IRQ on falling edge (button press)
    if ((err = request_irq(irq_number, button_isr, IRQF_TRIGGER_FALLING, "button_irq", NULL))) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpio_free(BUTTON_GPIO);
        return err;
    }

    if(request_irq(irq_number, (irq_handler_t) button_signal_handler, IRQF_TRIGGER_RISING, "button_irq_signal", NULL) != 0)
    {
		printk("Error!\nCan not request interrupt nr.: %d\n", irq_number);
		gpio_free(BUTTON_GPIO);
		return -1;
	}

	if(register_chrdev(BUTTON_MAJOR, "gpio_irq_signal", &fops) < 0) 
    {
		printk("Error!\n Can't register device Number!\n");
		gpio_free(BUTTON_GPIO);
		free_irq(irq_number, NULL);
	}


    printk(KERN_INFO "Button module initialized\n");

    return 0;
}

static void __exit button_exit(void)
{
    // Free the IRQ and GPIO
    free_irq(gpio_to_irq(BUTTON_GPIO), NULL);
    gpio_free(BUTTON_GPIO);

    printk(KERN_INFO "Button module removed\n");
}

module_init(button_init);
module_exit(button_exit);

