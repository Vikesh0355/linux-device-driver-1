#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ipc_mmap"
#define SHARED_MEM_SIZE PAGE_SIZE

MODULE_LICENSE("GPL");

static int major;
static char *shared_buffer = NULL;

static int ipc_mmap_open(struct inode *inode, struct file *filp) {
    return 0;
}

static int ipc_mmap_release(struct inode *inode, struct file *filp) {
    return 0;
}

static int ipc_mmap_mmap(struct file *filp, struct vm_area_struct *vma) {
    unsigned long pfn = virt_to_phys(shared_buffer) >> PAGE_SHIFT;

    return remap_pfn_range(vma,
                           vma->vm_start,
                           pfn,
                           vma->vm_end - vma->vm_start,
                           vma->vm_page_prot);
}

static struct file_operations ipc_fops = {
    .owner = THIS_MODULE,
    .open = ipc_mmap_open,
    .release = ipc_mmap_release,
    .mmap = ipc_mmap_mmap,
};

static int __init ipc_mmap_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &ipc_fops);
    if (major < 0) {
        pr_err("Failed to register device\n");
        return major;
    }

    shared_buffer = (char *)kmalloc(SHARED_MEM_SIZE, GFP_KERNEL);
    if (!shared_buffer) {
        unregister_chrdev(major, DEVICE_NAME);
        return -ENOMEM;
    }

    memset(shared_buffer, 0, SHARED_MEM_SIZE);
    pr_info("ipc_mmap device registered with major %d\n", major);
    return 0;
}

static void __exit ipc_mmap_exit(void) {
    kfree(shared_buffer);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("ipc_mmap device unregistered\n");
}

module_init(ipc_mmap_init);
module_exit(ipc_mmap_exit);
