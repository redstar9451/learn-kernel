#include <linux/init.h>
#include <linux/module.h>
#include "sub.h"

MODULE_LICENSE("GPL");

static __init int hello_init(void)
{
	printk(KERN_ERR "Hello World enter\n");
    func_a();
    func_b();
    return 0;
}

static __exit void hello_exit(void)
{
	printk(KERN_ERR "Hello World exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("redstar");
MODULE_DESCRIPTION("This is a test module");
MODULE_ALIAS("A Sample module");
