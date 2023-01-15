#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

// This kernel module will register an interrupt request (IRQ) on the falling edge (i.e., button press) of the specified GPIO pin. When the button is pressed, the button_isr function will be called and a message will be printed to the kernel log.

// Ensure button is always on this GPIO pin, if it's not change this value
#define BUTTON_GPIO 15

static int button_pressed = 0;

// This will be called when the button is pressed
static irqreturn_t button_isr(int irq, void *data)
{
    if(!button_pressed){
        printk(KERN_INFO "Button press detected\n");
        button_pressed = 1;
    }
    return IRQ_HANDLED;
}

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

    // Enable IRQ on falling edge (button press)
    if ((err = request_irq(gpio_to_irq(BUTTON_GPIO), button_isr,
                    IRQF_TRIGGER_FALLING, "button_irq", NULL))) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpio_free(BUTTON_GPIO);
        return err;
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Isaac Basque-Rice");
MODULE_DESCRIPTION("A simple Linux kernel module to detect button press on a Raspberry Pi");

