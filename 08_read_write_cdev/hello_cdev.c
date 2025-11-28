#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> // For creating device file
#include <linux/kernel.h>

static int major;
static char text[64];

static ssize_t my_read(struct file *fp, char __user *user_buf, size_t length, loff_t *offset)
{
    int not_copied, delta, to_copy = (length + *offset) < sizeof(text) ? length : (sizeof(text) - *offset);
    pr_info("hello_cdev: Read is called, we want to read %ld bytes, but actually only copying %d bytes. The offset is %lld\n", length, to_copy, *offset);
    if (*offset >= sizeof(text))
        return 0;

    not_copied = copy_to_user(user_buf, &text[*offset], to_copy);
    delta = to_copy - not_copied;
    if (not_copied)
        pr_warn("hello_cdev: Could only copy %d bytes\n", delta);
    *offset += delta;
    return delta;
}

static ssize_t my_write(struct file *fp, const char __user *user_buf, size_t length, loff_t *offset)
{
    int not_copied, delta, to_copy = (length + *offset) < sizeof(text) ? length : (sizeof(text) - *offset);

    pr_info("hello_cdev: Write is called, we want to write %ld bytes, but actually only copying %d bytes. The offset is %lld\n", length, to_copy, *offset);

    if (*offset >= sizeof(text))
        return 0;
    
    not_copied = copy_from_user(&text[*offset], user_buf, to_copy);
    delta = to_copy - not_copied;
    if (not_copied)
        pr_warn("hello_cdev: Could only copy %d bytes\n", delta);
    *offset += delta;
    return delta;
}

static struct file_operations fops = {
    .read = my_read,
    .write = my_write
};

static int my_init(void)
{
    major = register_chrdev(0, "hello_cdev", &fops); // Can specify device number but putting 0 finds a free device number and allocates that
    if (major < 0)
    {
        printk(KERN_ERR "hello_cdev: Error registering chrdev\n");
        return major;
    }
    printk(KERN_INFO "hello_cdev: Major Device Number: %d\n", major);

    printk(KERN_INFO "hello_cdev: Hello cdev kernel module loaded\n");
    return 0;
}

static void my_exit(void)
{
    unregister_chrdev(major, "hello_cdev");
    printk(KERN_INFO "hello_cdev: Hello cdev kernel module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A sample driver that registers a character device and performs open and release operation");
MODULE_AUTHOR("Nazib Abrar");