#include <linux/printk.h>
#include "sub1-1.h"

int func_a(void)
{
    func_c();
    printk (KERN_ERR "This is %s\n", __FUNCTION__);
    return 0;
}
