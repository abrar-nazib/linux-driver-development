#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h> // For SPI bus access

#define BUS_NUM 0

static char mod_name[20] = "spi_rc522";

static int mod_init(void){
    pr_info("%s: Driver Module Loaded", mod_name);    
    return 0;
}

static void mod_exit(void){
    pr_info("%s: Driver Module Unloaded", mod_name);
}

module_init(mod_init);
module_exit(mod_exit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple driver for RC522 RFID reader/writer via SPI bus");
MODULE_AUTHOR("Nazib Abrar");