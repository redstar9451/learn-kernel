/*
	This module is used to call functions in kernel, to display variables and set a new value.
	with the mips-arch board I had tested, neithor cat /proc/kallsyms nor kallsyms_lookup_name
	can get the address for global vairables in kernel,
	I don't test on other mips boards, not sure this module can display global variables or set new values.
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kallsyms.h>
#include <linux/ctype.h>
#include <linux/parser.h>
#include <linux/uaccess.h>

#define DEBUG_BUF_SIZE 128
#define PARAMS_NUM 8

struct kdbg_cdev
{
	struct cdev chrdev;
};

struct kdbg_cdev kdbg_cdev;


void kdbg_help (void)
{
	char kdgb_help_str[] = "usage:\n" \
							"echo call <func>,<param0>,<param1>,<param2>, ... > /dev/kdbg\n" \
							"echo x <symbol_name>,<size> > /dev/kdbg\n"\
							"echo x <address>,<size> > /dev/kdbg\n" \
							"echo set <address>,<value> > /dev/kdbg";

	printk(KERN_ERR "%s\n", kdgb_help_str);
}

typedef unsigned int (*KDBG_FUNC_PTR) (void *arg0, void *arg1,
												void *arg2, void *arg3,
												void *arg4, void *arg5,
												void *arg6, void *arg7);

char *str_trim(char *str)
{
	char *begin = NULL;
	char *tail = NULL;

	if (!str)
		return str;

	begin = str ;
	while (*begin && (isspace(*begin) || *begin == '"')) /* strip quote character, it is different with the classical string trim*/
		begin++;

	tail = begin + strlen(begin) - 1;
	while (tail > begin && (isspace(*tail) || *begin == '"'))
		*tail = '\0';

	return begin;
}

int parse_interger(char *str, unsigned int *result)
{
	substring_t substr ;

	if (!str)
		return -1;

	substr.from = str;
	substr.to = str + strlen(str);
	/* match_hex("debug_xxxx") return 0xdef, the prefix "0x" is used to identify the heximal interger */
	if ( (str[0]=='0' && tolower(str[1]) == 'x')
		 && match_hex(&substr, result) == 0)
	{
		return 0;
	}

	if ( match_int(&substr, result) == 0 )
		return 0;

	return -1;
}

int parse_address(char *str, unsigned char **addr)
{
	unsigned int result = 0;

	if (!str)
		return -1;

	if (parse_interger (str, &result) == 0)
	{
		*addr = (unsigned char *)result;
		return 0;
	}
/* there are 3 methods to lookup address for a symbol, kallsyms_lookup_name searchs more symbols than the other two methods
	but kallsyms_lookup_name is not exported, EXPORT_SYMBOL(kallsyms_lookup_name) firstly.
*/
#if 0
	*addr = (typeof(*addr))__symbol_get(func_name);
	*addr = (typeof(*addr))find_symbol(func_name, NULL, NULL, true, true);
#endif
	*addr = (typeof(*addr))kallsyms_lookup_name(str);
	if (*addr != 0)
		return 0;

	return -1;
}

unsigned int parse_params(char *params_str, void *param_array[])
{
	char *p = NULL;
	unsigned int count = 0;
	char *temp = 0;
	unsigned int result = 0;

	p = params_str;
	count = 0;
	while (p)
	{
		temp = strsep(&p, ",");
		temp = str_trim (temp);
		if (parse_interger (temp, &result) == 0)
			param_array[count++] = (void *)result;
		else
			param_array[count++] = temp;

		if (count == PARAMS_NUM)
			break;
	}
	return count;
}

void debug_call(char *str)
{
	char *func_name = NULL;
	char *p = str;
	unsigned char * addr = 0;
	KDBG_FUNC_PTR kdbg_func_ptr = NULL;
	unsigned int ret = 0;
	void *params[PARAMS_NUM] = {0};
	int err = 0;

	func_name = strsep(&p, ",");
	func_name = str_trim(func_name);
	err = parse_address(func_name, &addr);
	if(err)
	{
		printk(KERN_ERR "failed to parse the function address named %s\n", func_name);
		return;
	}

	memset(params, 0, sizeof(params));
	parse_params (p, params);
	kdbg_func_ptr = (KDBG_FUNC_PTR)addr;
	ret = (*kdbg_func_ptr)(params[0], params[1],
							params[2], params[3],
							params[4], params[5],
							params[6], params[7]);

	printk(KERN_ERR "%s return %d (%#x)\n", func_name, ret, ret);
}

void debug_x(char *str)
{
	char *symbol_or_addr = NULL;
	char *size_str = NULL;
	char *p = str;
	unsigned char *addr = NULL;
	unsigned int size = 0;
	int err = 0;

	symbol_or_addr = strsep(&p, ",");
	symbol_or_addr = str_trim(symbol_or_addr);
	err = parse_address (symbol_or_addr, &addr);
	if (err)
	{
		printk(KERN_ERR "failed to parse the address involved with %s\n", symbol_or_addr);
		return;
	}
	size_str = strsep(&p, ",");
	size_str = str_trim(size_str);
	err = parse_interger (size_str, &size);
	if(err)
		size = 0x4; /* 4 bytes by default */

	printk(KERN_DEBUG "addr: %#x, size: %d\n", (unsigned int)addr, size);
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4,addr, size, 1);
}

void debug_set(char *str)
{
	char *p = str;
	char *symbol_or_addr = NULL;
	unsigned char *addr = NULL;
	char *value_str = NULL;
	unsigned int value = 0;
	int err = 0;

	symbol_or_addr = strsep(&p, ",");
	symbol_or_addr = str_trim(symbol_or_addr);
	err = parse_address (symbol_or_addr, &addr);
	if (err)
	{
		printk(KERN_ERR "failed to parse the address for %s\n", symbol_or_addr);
		return;
	}
	value_str = strsep(&p, ",");
	value_str = str_trim(value_str);
	err = parse_interger (value_str, &value);
	if (err)
	{
		printk(KERN_ERR "failed to parse the value for %s\n", symbol_or_addr);
		return;
	}
	*((unsigned int *)addr) = value;
}

int kdbg_open (struct inode *inode, struct file *filep)
{
	return 0;
}

int kdbg_release (struct inode *inode, struct file *filep)
{
	return 0;
}

ssize_t kdbg_read (struct file *filep, char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

ssize_t kdbg_write (struct file *filep, const char __user *buf, size_t count, loff_t *ppos)
{
	char cmd_buf[DEBUG_BUF_SIZE] = {0};
	char *p = cmd_buf;
	char *cmd = NULL;
	int rv = 0;

	if (count >= DEBUG_BUF_SIZE)
	{
		printk(KERN_INFO "The cmd is long than %d\n", DEBUG_BUF_SIZE);
		return -1;
	}
	memset(cmd_buf, 0, sizeof(cmd));
	rv = copy_from_user(cmd_buf, buf, count);
	if (rv)
	{
		printk(KERN_ERR "failed to copy from user rv = %d count = %d\n", rv, count);
		return -EIO;
	}

	if (cmd_buf[count - 1] == '\n');
		cmd_buf[count - 1] = '\0';

	cmd = strsep (&p, " ");
	if (0 == strcmp(cmd, "call"))
		debug_call(p);
	else if (0 == strcmp(cmd, "x"))
		debug_x(p);
	else if (0 == strcmp(cmd, "set"))
		debug_set(p);
	else
		kdbg_help();
	return count;
}

long kdbg_ioctl (struct file *filep, unsigned int cmd, unsigned long args)
{
	return 0;
}

struct file_operations kdbg_fops =
{
	.owner			= THIS_MODULE,
	.open 			= kdbg_open,
	.release 		= kdbg_release,
	.read 			= kdbg_read,
	.write			= kdbg_write,
	.compat_ioctl	= kdbg_ioctl,
};

static void kdbg_setup_cdev(struct kdbg_cdev *dev, struct file_operations *fops)
{
	int err = 0;
	dev_t devno = 0;

	err = alloc_chrdev_region (&devno, 0, 1, "kdbg");
	printk(KERN_DEBUG "KDBG module device number, MAJOR: %d MINOR: %d\n", MAJOR(devno), MINOR(devno));
	cdev_init(&dev->chrdev, fops);
	dev->chrdev.owner = THIS_MODULE;
	dev->chrdev.ops = fops;
	err = cdev_add(&dev->chrdev, devno, 1);
	if(err)
		printk(KERN_NOTICE "Error %d adding kdbg device\n", err);
}

static void kdbg_unregister_cdev(struct kdbg_cdev *dev)
{
	unregister_chrdev_region (dev->chrdev.dev, dev->chrdev.count);
	cdev_del(&dev->chrdev);
	return;
}

static int __init kdbg_module_init(void)
{
	kdbg_setup_cdev(&kdbg_cdev, &kdbg_fops);
	return 0;
}

static void __exit kdbg_module_exit(void)
{
	kdbg_unregister_cdev(&kdbg_cdev);
}

unsigned int kdbg_test_data = 0xA050;

unsigned int kdbg_call_test_01(unsigned int param1, unsigned int param2, char * param3)
{
	printk(KERN_ERR "%d %d %s\n", param1, param2, param3);
	return 0;
}

void kdbg_call_test_02(char *param1, unsigned int param2, unsigned int param3)
{
	printk(KERN_ERR "%s %d %d\n", param1, param2, param3);
}

void kdbg_call_test_03(char *param1, unsigned int param2, unsigned int param3)
{
	param1 = 0;
	*param1 = 'c';
	printk(KERN_ERR "%s %d %d\n", param1, param2, param3);
}

module_init(kdbg_module_init);
module_exit(kdbg_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("redstar");
MODULE_DESCRIPTION("debug kernel code");
