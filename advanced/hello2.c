// SPDX-License-Identifier: GPL-2.0+

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>

#include "hello1.h"

extern int print_hello(uint);

static uint howmany = 1;
module_param(howmany, uint, 0660);
MODULE_PARM_DESC(howmany, "A parameter that determines how many times the message will be displayed. Accepts values from 0-10.");

static int __init hello2_init(void)
{
	return print_hello(howmany);
}

static void __exit hello2_exit(void)
{
	/* Do nothing here right now */
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_AUTHOR("Herman Ananenko <example@exmpl.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training. Part 2.");
MODULE_LICENSE("Dual BSD/GPL");
