#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>

MODULE_LICENSE("Dual BSD/GPL");

/* notifier chain常用的封装套路：
 * 1. 定义 notifier chain: atomic, blocking, raw, rcu
 * 2. 使用(un)register_xxx_notifier(struct notifier_block * nb), notify_die(unsigned long action, void *data, ...)
 *    封装调用链，即把notifier_head屏蔽掉
 *
 * Tips:
 * 1. 注意notifier chain在SMP环境下可由多个cpu同时调用，回调函数要处理互斥问题 */

int callback_atomic_notifer(struct notifier_block *nb, unsigned long action, void *data)
{
	printk(KERN_ERR "%s %ld %d\n", __FUNCTION__, action, (int)data);
	return 0;
}

int callback_atomic_notifer_02(struct notifier_block *nb, unsigned long action, void *data)
{
	printk(KERN_ERR "%s %ld %d\n", __FUNCTION__, action, (int)data);
	return 0;
}

struct notifier_block test_atomic_notifier = {
	.notifier_call = callback_atomic_notifer,
};

struct notifier_block test_atomic_notifier_02 = {
	.notifier_call = callback_atomic_notifer_02,
};
/* 除了RCU类型的notifier外，notifer有两种定义和初始化方法：
 * 非结构体成员，定义和初始化使用一个宏搞定，这是常用的方法. XXX_NOTIFIER_HEAD()
 * 结构体成员一般不在定义时初始化，所以定义还是传统的定义方法，初始化使用 XXX_INIT_NOTIFIER_HEAD() 宏搞定 */
//ATOMIC_NOTIFIER_HEAD(atomic_head);

struct atomic_notifier_head atomic_head;

int notifier_my_test_register(void)
{
	atomic_notifier_chain_register(&atomic_head, &test_atomic_notifier);
	atomic_notifier_chain_register(&atomic_head, &test_atomic_notifier_02);
	return 0;
}

int notifier_my_test_call(unsigned long action, void *value)
{
	atomic_notifier_call_chain(&atomic_head, action, value);
	return NOTIFY_OK;
}

int notifier_my_test_unregister(void)
{
	atomic_notifier_chain_unregister(&atomic_head, &test_atomic_notifier);
	atomic_notifier_chain_unregister(&atomic_head, &test_atomic_notifier_02);
	return 0;
}

static __init int notifier_init(void)
{
	ATOMIC_INIT_NOTIFIER_HEAD(&atomic_head);
    return 0;
}

static __exit void notifier_exit(void)
{

}

module_init(notifier_init);
module_exit(notifier_exit);
MODULE_AUTHOR("redstar");
MODULE_DESCRIPTION("Slab test module");
MODULE_ALIAS("A Sample module");
