// SPDX-License-Identifier: GPL-2.0+

/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;
module_param(howmany, uint, 0660);
MODULE_PARM_DESC(howmany, "A parameter that determines how many times the message will be displayed. Accepts values from 0-10.");

struct data {
	ktime_t time;
	struct list_head list;
};

struct list_head *iter, *iter_safe;
struct data *item;
LIST_HEAD(list);

static int __init hello_init(void)
{
	int i;

	if (howmany >= 0 && howmany <= 10) {
		if (howmany == 0)
			pr_warn("Small value!\n");
		else if (howmany > 5 && howmany <= 10)
			pr_warn("Big value!\n");

		for (i = 0; i < howmany; i++) {
			item = kmalloc(sizeof(*item), GFP_KERNEL);
			item->time = ktime_get_ns();
			list_add_tail(&(item->list), &list);
			pr_alert("Hello, world!\n");
		}
	} else {
		pr_err("Invalid value!\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit hello_exit(void)
{
	list_for_each_safe(iter, iter_safe, &list) {
		item = list_entry(iter, struct data, list);
		pr_info("%lu\n", (unsigned long) item->time);
		list_del(iter);
		kfree(item);
	}
}

module_init(hello_init);
module_exit(hello_exit);
