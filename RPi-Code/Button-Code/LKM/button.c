#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

// NOTE: this was created outwith a Raspberry Pi environent, it may need editing when it is introduced there 
// This kernel module will register an interrupt request (IRQ) on the falling edge (i.e., button press) of the specified GPIO pin. When the button is pressed, the button_isr function will be called and a message will be printed to the kernel log.

// Replace with the actual GPIO number
#define BUTTON_GPIO 17

// This will be called when the button is pressed
static irqreturn_t button_isr(int irq, void *data)
{
    printk(KERN_INFO "Button press detected\n");
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

