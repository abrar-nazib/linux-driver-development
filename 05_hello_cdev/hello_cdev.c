#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> // For creating device file
#include <linux/kernel.h> 

static int major;

static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o){
    printk(KERN_INFO "hello_cdev: Read is called\n");
    return 0;
}

static struct file_operations fops = {
    .read = my_read
};

static int my_init(void){
    major = register_chrdev(0, "hello_cdev", &fops); // Can specify device number but putting 0 finds a free device number and allocates that
    if(major < 0){
        printk(KERN_ERR "hello_cdev: Error registering chrdev\n");
        return major;
    }
    printk(KERN_INFO "hello_cdev: Major Device Number: %d\n", major);
    
    printk(KERN_INFO "hello_cdev: Hello cdev kernel module loaded\n"); 
    return 0;
}

static void my_exit(void){
    unregister_chrdev(major, "hello_cdev");
    printk(KERN_INFO "hello_cdev: Hello cdev kernel module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A sample driver that registers a character device");
MODULE_AUTHOR("Nazib Abrar");