#include <linux/printk.h>

int func_c(void)
{
    printk (KERN_ERR "This is %s\n", __FUNCTION__);
    return 0;
}
