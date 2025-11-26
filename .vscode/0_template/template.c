#include <linux/module.h>
#include <linux/init.h>

static char mod_name[20] = "";

static int mod_init(void){
    pr_info("%s: Driver module Loading..\n",mod_name);

    pr_info("%s: Driver Module Loaded\n", mod_name);    
    return 0;
}

static void mod_exit(void){
    pr_info("%s: Unloading Driver Module...\n", mod_name);

    pr_info("%s: Driver Module Unloaded\n", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver ");
MODULE_AUTHOR("Nazib Abrar");