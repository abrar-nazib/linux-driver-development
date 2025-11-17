#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h> // For KERN_INFO

static int my_init(void){
    printk(KERN_INFO "Hello world kernel module loaded\n"); // Printf doesn't work. We're writing to kernel log. The trailing \n is important
    return 0;
}

static void my_exit(void){
    printk(KERN_INFO "Hello world kernel module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple hello world module");
MODULE_AUTHOR("You");