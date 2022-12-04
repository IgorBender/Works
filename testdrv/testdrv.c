/*  testdrv.c - Kernel module implementing user access to PS DMAs of MpSoc/RfSoC.

* Copyright (C) 2022 Igor Bender 
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.

*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program. If not, see <http://www.gnu.org/licenses/>.

*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/kthread.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include "testdrv-ioctl.h"

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Igor Bender");
MODULE_DESCRIPTION("testdrv - user access ioctl test");

#define DRIVER_NAME "testdrv"
#define DRIVER_VERSION_MAJOR 1
#define DRIVER_VERSION_MINOR 0
#define testdrv_DEVICE_NAME "testdrv"
#define NUMBER_TESTDRV_CHANNELS 4
#define TESTDRV_MEM_SIZE 4096

static const char class_name[] = "testdrvclass";
static dev_t dev;
static struct class* class;
static int major;

struct testdrv_data
{
    int dev_num;
    struct cdev cdev;
    struct device* device;
    char* p_mem;
    u32 offset;
    u32 count;
};

static struct testdrv_data device_data[NUMBER_TESTDRV_CHANNELS];

static int my_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

int testdrv_open(struct inode *i_node, struct file *filp)
{
    int minor = iminor(i_node);

    struct testdrv_data* p_data = &device_data[minor];
    dev_warn(p_data->device, " open device %d\n", minor);
//    p_lp = container_of(i_node->i_cdev, struct reg_access_local, cdev);
    filp->private_data = p_data;
    filp->f_pos = 0;

    return 0;
}

int testdrv_release(struct inode *i_node, struct file *filp)
{
    int minor = iminor(i_node);

    struct testdrv_data* p_data = &device_data[minor];
    dev_warn(p_data->device, " close device %d\n", minor);
    return 0;
}

long testdrv_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    int minor;
    struct testdrv_data* p_data;
    int res;

    minor = iminor(f->f_inode);
    p_data = &device_data[minor];

    switch(cmd)
    {
		case TESTDRV_GET_DRIVER_VERSION:
		{   
			struct Version Ver;
			dev_warn(p_data->device, " ioctl get version\n");

			Ver.Major = DRIVER_VERSION_MAJOR;
			Ver.Minor = DRIVER_VERSION_MINOR;
			res = copy_to_user((void*)arg, &Ver, sizeof(struct Version));
			break;
		}
		
		case TESTDRV_ZERO_MEMORY:
		{
			dev_warn(p_data->device, " ioctl zero memory\n");

			if(!p_data->p_mem)
				return -EFAULT;
			
			memset(p_data->p_mem, 0, TESTDRV_MEM_SIZE);
			break;
		}

		case TESTDRV_FILL_MEMORY:
		{
			dev_warn(p_data->device, " ioctl fill char %d\n", (u8)arg);

			if(!p_data->p_mem)
				return -EFAULT;
			
			memset(p_data->p_mem, (u8)arg, TESTDRV_MEM_SIZE);
			break;            
		}
		
		default:
			return -ENOTTY;
    }

    return 0;
}

int testdrv_mmap(struct file *filp, struct vm_area_struct *vma)
{
    int minor;
    struct testdrv_data* p_data;
    unsigned long size = vma->vm_end - vma->vm_start;
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long pfn;


    minor = iminor(filp->f_inode);
    p_data = &device_data[minor];

    if(size + offset > TESTDRV_MEM_SIZE)
        return -EINVAL;

    pfn = virt_to_phys(p_data->p_mem) >> PAGE_SHIFT;
    vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
    if(remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot))
    {
		dev_warn(p_data->device, " mmap() error\n");
        return -EAGAIN;
    }
    dev_warn(p_data->device, " mmap() success\n");
    return 0;
}

ssize_t testdrv_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct testdrv_data *lp;
    int size;
    int res;
    loff_t offset = *f_pos;

    lp = filp->private_data;


    if(count > TESTDRV_MEM_SIZE - offset)
        size = TESTDRV_MEM_SIZE - offset;
    else
        size = count;
    dev_warn(lp->device, " write %d bytes at offset %lld\n", size, offset);

    res = copy_from_user(lp->p_mem + offset, buf, size);
    if(0 != res)
    {
	*f_pos += size - res;
	return size - res;
    }
    *f_pos += size;
    return size;
}

ssize_t testdrv_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct testdrv_data *lp;
    int size;
    int res;
    loff_t offset = *f_pos;
    lp = filp->private_data;

    
    if(count > TESTDRV_MEM_SIZE - offset)
        size = TESTDRV_MEM_SIZE - offset;
    else
        size = count;
    dev_warn(lp->device, " read %d bytes at offset %lld\n", size, offset);
    res = copy_to_user(buf, lp->p_mem + offset, size);
    if(0 != res)
    {
        *f_pos += size - res;
        return size - res;
    }
    *f_pos += size;
    return size;
}

loff_t testdrv_llseek(struct file *filp, loff_t off, int whence)
{
	struct testdrv_data *lp;
	loff_t newpos;

   	lp = filp->private_data;

	switch(whence) {
	  case 0: /* SEEK_SET */
		dev_warn(lp->device, " set file offset %lld bytes from begin\n", off);
		newpos = off;
		break;

	  case 1: /* SEEK_CUR */
		dev_warn(lp->device, " set file offset %lld bytes from current position\n", off);
		newpos = filp->f_pos + off;
		break;

	  case 2: /* SEEK_END */
		dev_warn(lp->device, " set file offset %lld bytes from end\n", off);
		newpos = TESTDRV_MEM_SIZE + off;
		break;

	  default: /* can't happen */
		return -EINVAL;
	}
	if (newpos < 0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}

struct file_operations testdrv_fops = {
    .owner =    		THIS_MODULE,
    .read =     		testdrv_read,
    .write =    		testdrv_write,
    .open =     		testdrv_open,
    .release =  		testdrv_release,
    .mmap = 			testdrv_mmap,
    .unlocked_ioctl =	testdrv_ioctl,
    .llseek =   		testdrv_llseek,
};

static int __init testdrv_init(void)
{
    int err = 0;
    u32 chan_id = 0;
    int devno;
    struct device* p_dev;
    int rc = 0;

    memset(device_data, 0, sizeof(struct testdrv_data) * NUMBER_TESTDRV_CHANNELS);
    err = alloc_chrdev_region(&dev, 0, NUMBER_TESTDRV_CHANNELS, testdrv_DEVICE_NAME);
    if (err < 0) {
//		dev_warn(dev, "alloc_chrdev_region() failed\n");
        return err;
    }
    class = class_create(THIS_MODULE, class_name);
    if(NULL == class)
    {
//	    dev_warn(dev, "class_create() failed\n");
        return -EINVAL;
    }
    class->dev_uevent =	my_dev_uevent;
    major = MAJOR(dev);

	for(chan_id = 0; chan_id < NUMBER_TESTDRV_CHANNELS; ++chan_id)
	{
		devno = MKDEV(major, chan_id);
		cdev_init(&device_data[chan_id].cdev, &testdrv_fops);
		device_data[chan_id].cdev.owner = THIS_MODULE;
		device_data[chan_id].cdev.ops = &testdrv_fops;
		cdev_add(&device_data[chan_id].cdev, devno, 1);
		device_data[chan_id].dev_num = chan_id;
		device_data[chan_id].p_mem = (char*)kmalloc(TESTDRV_MEM_SIZE, GFP_KERNEL);
		memset(device_data[chan_id].p_mem, 0, TESTDRV_MEM_SIZE);
		p_dev = device_create(class, NULL, devno, &device_data[chan_id], "testdrv%d", chan_id);
		if(NULL == p_dev)
		{
			dev_err(p_dev, "create_device() failed");
			rc = -EINVAL;
			goto error1;
		}
		dev_warn(p_dev, "create_device() for %d\n", chan_id);
		device_data[chan_id].device = p_dev;
	}
	return 0;

error1:
	return rc;
}

static void __exit testdrv_exit(void)
{
	int i;
	int devno;
	
	for(i = 0; i < NUMBER_TESTDRV_CHANNELS; ++i)
	{
	    devno = MKDEV(major, i);
	    dev_warn(device_data[i].device, "destroy_device() %d\n", i);    
	    cdev_del(&device_data[i].cdev);
	    device_destroy(class, devno);
	    kfree(device_data[i].p_mem);
	}
	class_destroy(class);
	unregister_chrdev_region(dev, NUMBER_TESTDRV_CHANNELS);
	memset(device_data, 0, sizeof(device_data));
}

module_init(testdrv_init);
module_exit(testdrv_exit);
