#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/of.h>           

#define LED_GPIO    21   
#define BUTTON_GPIO 20   
#define OFFSET 512

static struct gpio_desc *led_desc = NULL;
static struct gpio_desc *button_desc = NULL;

static int my_init(void)
{
    printk(KERN_INFO "gpioctrl: GPIO Control module loading...\n");

    led_desc = gpio_to_desc(LED_GPIO + OFFSET);
    if (!led_desc || IS_ERR(led_desc)) {
        printk(KERN_ERR "gpioctrl: Failed to get GPIO 21 descriptor\n");
        return -ENODEV;
    }

    button_desc = gpio_to_desc(BUTTON_GPIO + OFFSET);
    if (!button_desc || IS_ERR(button_desc)) {
        printk(KERN_ERR "gpioctrl: Failed to get GPIO 20 descriptor\n");
        gpiod_put(led_desc);
        return -ENODEV;
    }

    if (gpiod_direction_output(led_desc, 0)) {
        printk(KERN_ERR "gpioctrl: Failed to set GPIO 21 as output\n");
        gpiod_put(led_desc);
        gpiod_put(button_desc);
        return -ENODEV;
    }

    if (gpiod_direction_input(button_desc)) {
        printk(KERN_ERR "gpioctrl: Failed to set GPIO 20 as input\n");
        gpiod_put(led_desc);
        gpiod_put(button_desc);
        return -ENODEV;
    }

    gpiod_set_value(led_desc, 1);
    printk(KERN_INFO "gpioctrl: LED on GPIO21 turned ON\n");
    printk(KERN_INFO "gpioctrl: Button on GPIO20 state: %d\n", gpiod_get_value(button_desc));

    return 0;
}

static void my_exit(void)
{
    if (led_desc) {
        gpiod_set_value(led_desc, 0);
        gpiod_put(led_desc);
    }
    if (button_desc)
        gpiod_put(button_desc);

    printk(KERN_INFO "gpioctrl: GPIO Control module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple GPIO LED + Button test");
MODULE_AUTHOR("Nazib Abrar");