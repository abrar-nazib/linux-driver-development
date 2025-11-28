#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include "my_ioctl.h"

static char *mod_name = "ioctl";
#define DEVICE_NAME "ioctl" // -> Creates /dev/hello_cdev
#define CLASS_NAME "ioctl"

static dev_t devnr; // Will hold major and minor
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static int answer = 42;
static long int my_ioctl(struct file *f, unsigned int cmd, unsigned long args)
{
    int status;
    struct mystruct s;
    pr_info("%s: Ioctl called with cmd: 0x%x and args %p\n", mod_name, cmd, (void *)args);

    switch (cmd)
    {
    case WR_VAL:
        status = copy_from_user(&answer, (int *)args, sizeof(int));
        if (status)
        {
            pr_err("%s: Error on WR_VAL\n", mod_name);
            return status;
        }
        break;

    case RD_VAL:
        status = copy_to_user((int *)args, &answer, sizeof(int));
        if (status)
        {
            pr_err("%s: Error on RD_VAL\n", mod_name);
            return status;
        }
        break;

    case GREET:
        status = copy_from_user(&s, (struct mystruct *)args, sizeof(struct mystruct));
        if (status)
        {
            pr_err("%s: Error on GREET\n", mod_name);
            return status;
        }
        for (int i = 0; i < s.repeat; i++){
            pr_info("%s: Hello %s\n", mod_name, s.name);
        }
        break;

    default:
        return -EOPNOTSUPP; // Option not supported
    }

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl};

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

    pr_info("%s: Created device /dev/%s\n", mod_name, mod_name);
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