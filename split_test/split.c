#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

char* str = "21,1";    
char* d = ",";

typedef struct LED {
    int pin;
    int mode;
} LED;

static int __init _init(void) {           
    LED led;
    char[] p;

    printk(KERN_INFO "running strsep\n");

    char* copy = (char*)kzalloc(sizeof(str), GFP_KERNEL);
    strcpy(copy, str);

    strncpy(p, )
    strsep(&copy, d);
    
    kstrtoint(copy, 0, &led.pin);

    strsep(&copy, d);
    kstrtoint(copy, 0, &led.mode);

    printk(KERN_INFO "pin=%d, mode=%d", led.pin, led.mode);
    printk(KERN_INFO "Init OK!\n");
    return 0;
}



static void __exit _exit(void)
{   
    printk(KERN_INFO "exiting module\n");    
}

module_init(_init);
module_exit(_exit);
MODULE_LICENSE("GPL");