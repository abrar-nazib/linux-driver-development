#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> // For creating device file
#include <linux/kernel.h> 

static int major;

static int my_open (struct inode *inode, struct file *filep){
    pr_info("hello_cdev: Major: %d, Minor: %d\n", imajor(inode), iminor(inode));
    
    // Print out some flags
    pr_info("hello_cdev: filp->f_pos: %lld\n", filep->f_pos);
    pr_info("hello_cdev: filp->f_mode: 0x%x\n", filep->f_mode);
    pr_info("hello_cdev: filp->f_flags: 0x%x\n", filep->f_flags);

    return 0;
}

static int my_release (struct inode *inode, struct file *filep){
    pr_info("hello_cdev: File is closed");
    return 0;
}



static struct file_operations fops = {
    .open = my_open,
    .release = my_release
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
MODULE_DESCRIPTION("A sample driver that registers a character device and performs open and release operation");
MODULE_AUTHOR("Nazib Abrar");