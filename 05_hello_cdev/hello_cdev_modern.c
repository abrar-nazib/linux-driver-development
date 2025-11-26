#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static char mod_name[20] = "hello_cdev_modern";

static dev_t devnr; // Will hold major and minor
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

#define DEVICE_NAME "hello_cdev" // -> Creates /dev/hello_cdev
#define CLASS_NAME "hello_class"

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    pr_info("%s: read() called\n", mod_name);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read
};

static int mod_init(void)
{
    pr_info("%s: Loading Driver Module...\n", mod_name);

    int ret;

    // Step 1: Allocate major/minor numbers
    ret = alloc_chrdev_region(&devnr, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        pr_err("%s: Failed to allocate major number\n", mod_name);
        return ret;
    }
    pr_info("%s: Allocated Major = %d, Minor = %d\n", mod_name, MAJOR(devnr), MINOR(devnr));

    // Step 2: Initialize cdev structure
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    // Step 3: Add the cdev to system
    ret = cdev_add(&my_cdev, devnr, 1);
    if (ret < 0)
    {
        pr_err("%s: Failed to add cdev\n", mod_name);
        goto err_unreg;
    }

    // Step 4: Create a class (appears in /sys/class/)
    my_class = class_create(CLASS_NAME);
        // Modern way: class_create(THIS_MODULE, "name")
    if (IS_ERR(my_class))
    {
        pr_err("%s: Could not create class\n", mod_name);
        ret = PTR_ERR(my_class);
        goto err_cdev;
    }

    // Step 5: Create device node /dev/hello_cdev
    my_device = device_create(my_class, NULL, devnr, NULL, DEVICE_NAME);
    if (IS_ERR(my_device))
    {
        pr_err("%s: Failed to create device file\n", mod_name);
        ret = PTR_ERR(my_device);
        goto err_class;
    }

    pr_info("%s: Created device /dev/hello_cdev\n", mod_name);
    pr_info("%s: Driver Module Loaded Successfully.\n", mod_name);
    return 0;

err_class:
    class_destroy(my_class);
err_cdev:
    cdev_del(&my_cdev);
err_unreg:
    unregister_chrdev_region(devnr, 1);
    return ret;
}

static void mod_exit(void)
{
    pr_info("%s: Unloading Driver Module....\n", mod_name);
    device_destroy(my_class, devnr);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(devnr, 1);
    pr_info("%s: Unloaded Driver Module Successfully.\n", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver ");
MODULE_AUTHOR("Nazib Abrar");