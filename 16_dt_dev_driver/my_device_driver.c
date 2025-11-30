#include <linux/module.h>
#include <linux/init.h>

/* Specific include for this code */
#include <linux/mod_devicetable.h> 
#include <linux/platform_device.h>
/* ------------ */

static char *mod_name = "dev_tree_driver";

/* Name all the compatible devices */
// of -> Open Firmware (Inventor of device tree)
static const struct of_device_id my_dev_ids[] = {
    {.compatible="cortextech,mydev"},
    {}/* Empty element needed to declare end of list. */
};

MODULE_DEVICE_TABLE(of, my_dev_ids); // Not required but used for automatic loading for device kernel modules

/* Implement a probe and remove function */
//Probe function is called when a compatible device is added. Opposite for the remove function.
static int my_dev_probe(struct platform_device *pdev){
    pr_info("%s: Probe function is called\n", mod_name);
    return 0;
}

static void my_dev_remove(struct platform_device *pdev){
    pr_info("%s: Remove function is called\n", mod_name);
}



/* Bundle compatible devices, probe and remove in driver's struct. */
static struct platform_driver my_dev_driver = {
    .probe = my_dev_probe,
    .remove = my_dev_remove,
    .driver = {
        .name = "my_dev_driver",
        .of_match_table = my_dev_ids,
    }
};

/* Register the device at the OS */
static int mod_init(void)
{
    pr_info("%s: Loading Driver Module...\n", mod_name);

    int return_val = platform_driver_register(&my_dev_driver);

    pr_info("%s: Driver Module Loaded Successfully.\n", mod_name);
    return return_val;

}

static void mod_exit(void)
{
    pr_info("%s: Unloading Driver Module....\n", mod_name);
    platform_driver_unregister(&my_dev_driver);
    pr_info("%s: Unloaded Driver Module Successfully.\n", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver device tree");
MODULE_AUTHOR("Nazib Abrar");