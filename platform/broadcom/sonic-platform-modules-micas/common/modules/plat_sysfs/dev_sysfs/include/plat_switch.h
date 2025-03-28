/*
 * A header definition for plat_switch driver
 *
 * Copyright (C) 2024 Micas Networks Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PLAT_SWITCH_H_
#define _PLAT_SWITCH_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/delay.h>

enum LOG_LEVEL{
    INFO = 0x1,
    ERR  = 0x2,
    DBG  = 0x4,
    ALL  = 0xf
};

#define LOG_INFO(_prefix, fmt, args...) \
            do { \
                if (g_loglevel & INFO) \
                { \
                    printk( KERN_INFO _prefix "%s "fmt, __FUNCTION__, ##args); \
                } \
            } while (0)

#define LOG_ERR(_prefix, fmt, args...) \
            do { \
                if (g_loglevel & ERR) \
                { \
                    printk( KERN_ERR _prefix "%s "fmt, __FUNCTION__, ##args); \
                } \
            } while (0)

#define LOG_DBG(_prefix, fmt, args...) \
            do { \
                if (g_loglevel & DBG) \
                { \
                    printk( KERN_DEBUG _prefix "%s "fmt, __FUNCTION__, ##args); \
                } \
            } while (0)

#define check_pfun(p) \
            do { \
                if (p == NULL) { \
                    printk( KERN_ERR "%s, %s = NULL.\n", __FUNCTION__, #p); \
                    return -ENOSYS; \
                } \
            }while(0)

#define check_p(p) check_pfun(p)

#define to_switch_obj(x) container_of(x, struct switch_obj, kobj)
#define to_switch_attr(x) container_of(x, struct switch_attribute, attr)
#define to_switch_device_attr(x) container_of(x, struct switch_device_attribute, switch_attr)

#define SWITCH_ATTR(_name, _mode, _show, _store, _type)    \
        { .switch_attr = __ATTR(_name, _mode, _show, _store),   \
          .type = _type }

#define SWITCH_DEVICE_ATTR(_name, _mode, _show, _store, _type) \
struct switch_device_attribute switch_dev_attr_##_name          \
        = SWITCH_ATTR(_name, _mode, _show, _store, _type)

struct switch_obj {
    struct kobject kobj;
    unsigned int index;
};

/* a custom attribute that works just for a struct switch_obj. */
struct switch_attribute {
    struct attribute attr;
    ssize_t (*show)(struct switch_obj *foo, struct switch_attribute *attr, char *buf);
    ssize_t (*store)(struct switch_obj *foo, struct switch_attribute *attr, const char *buf, size_t count);
};

struct switch_device_attribute {
    struct switch_attribute switch_attr;
    int type;
};

extern struct switch_obj *wb_plat_kobject_create(const char *name, struct kobject *parent);
extern void wb_plat_kobject_delete(struct switch_obj **obj);

#endif /* _PLAT_SWITCH_H_ */
