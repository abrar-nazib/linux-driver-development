#include <linux/module.h>
#include <linux/init.h>


static char *mod_name = "ioctl";

static int mod_init(void)
{
    pr_info("%s: Loading Driver Module...\n", mod_name);

    pr_info("%s: Driver Module Loaded Successfully.\n", mod_name);
    return 0;

}

static void mod_exit(void)
{
    pr_info("%s: Unloading Driver Module....\n", mod_name);

    pr_info("%s: Unloaded Driver Module Successfully.\n", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver ");
MODULE_AUTHOR("Nazib Abrar");