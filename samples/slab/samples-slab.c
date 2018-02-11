#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

struct kmem_cache *test_cache = NULL;
#define TEST_CACHE_SIZE 128
char *str[16] = {0};

void slab_ctor(void *data)
{
	static int index = 0;
	char *str = data;
	memset(str, 0, TEST_CACHE_SIZE);
	printk(KERN_ERR "slab actor, index=%d\n", index);
	sprintf(str, "Hello, this is a cache entry, index = %d", index++);
	return;
}

static __init int slab_init(void)
{
	test_cache = kmem_cache_create("test cache", TEST_CACHE_SIZE, 0, SLAB_HWCACHE_ALIGN|SLAB_PANIC, slab_ctor);
	if (!test_cache)
	{
		printk(KERN_ERR "fail to create slab cache\n");
		return 0;
	}
	printk (KERN_ERR "before kmem_cache_alloc\n");
	str[0] = kmem_cache_alloc(test_cache, GFP_KERNEL); /* 首次分配时，会预分配出多个，目前测试的情况为21个 */
	printk(KERN_ERR "after kmem_cache_alloc\n");
	printk(KERN_ERR "str: %s\n", str[0]);

	printk (KERN_ERR "before kmem_cache_alloc\n");
	str[1] = kmem_cache_alloc(test_cache, GFP_KERNEL);
	printk(KERN_ERR "after kmem_cache_alloc\n");
	printk(KERN_ERR "str: %s\n", str[1]);
    return 0;
}

static __exit void slab_exit(void)
{
	/* kmem_cache_destroy can handle NULL pointer, no need to validate the param*/
	kmem_cache_destroy(NULL);
	/* before destroy kmem cache, the cache will be destroyed must has no allocated entries,
     * or you will get a stack dump, and fail to destroy */
	kmem_cache_free(test_cache, str[0]);
	kmem_cache_free(test_cache, str[1]);
	kmem_cache_destroy(test_cache);
	printk(KERN_ERR "slab exit\n");
}

module_init(slab_init);
module_exit(slab_exit);
MODULE_AUTHOR("redstar");
MODULE_DESCRIPTION("Slab test module");
MODULE_ALIAS("A Sample module");
