#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

static char mod_name[20] = "gpio_irq";

#define BUTTON_GPIO_BCM 17
#define BUTTON_GPIO_GLOBAL (512 + 17)

static struct gpio_desc *button_desc = NULL;
static unsigned int irq_number = 0;

static irqreturn_t button_irq_handler(int irq, void *dev_id){
    pr_info("%s: Button pressed/released on GPIO%d! IRQ %d fired!\n", mod_name, BUTTON_GPIO_BCM, irq);
    return IRQ_HANDLED;
}

static int mod_init(void)
{
    pr_info("%s: Driver module Loading..\n", mod_name);
    int ret;

    button_desc = gpio_to_desc(BUTTON_GPIO_GLOBAL);
    if(!button_desc || IS_ERR(button_desc)){
        pr_err("%s: Failed to get GPIO %d\n", mod_name, BUTTON_GPIO_BCM);
        return -ENODEV; 
    }

    ret = gpiod_direction_input(button_desc);
    if(ret){
        pr_err("%s: Can't set GPIO%d as input\n", mod_name, BUTTON_GPIO_BCM);
        return ret;
    }

    gpiod_set_debounce(button_desc, 200000); // Debouncing the GPIO Pin for 20 ms

    irq_number = gpiod_to_irq(button_desc);
    if(irq_number < 0){
        pr_err("%s: GPIO%d cannot generate interrupts.\n", mod_name, BUTTON_GPIO_BCM);
    }

    pr_info("%s: GPIO%d -> IRQ %d\n", mod_name, BUTTON_GPIO_BCM, irq_number);

    ret = request_irq(irq_number, button_irq_handler, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "my_button_irq", NULL);

    if(ret){
        pr_err("%s: Failed to request IRQ %d\n",mod_name, irq_number);
        return ret;
    }

    pr_info("%s Module loaded successfully!\n", mod_name);
    return 0;
}

static void mod_exit(void)
{
    pr_info("%s: Unloading Driver Module...\n", mod_name);
    
    if(irq_number > 0)
        free_irq(irq_number, NULL);
    
    if(button_desc)
        gpiod_put(button_desc); // Release the GPIO

    pr_info("%s: Driver Module Unloaded\n", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver for GPIO Interrupt Control");
MODULE_AUTHOR("Nazib Abrar");