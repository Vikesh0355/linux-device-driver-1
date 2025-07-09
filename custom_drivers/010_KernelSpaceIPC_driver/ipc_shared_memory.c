#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "ipc_mmap"
#define SHARED_MEM_SIZE 4096

MODULE_LICENSE("GPL");

static int major;
static char *shared_buffer;
static size_t buffer_len = 0;

static int ipc_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "ipc_rw: Device opened\n");
    return 0;
}

static int ipc_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "ipc_rw: Device closed\n");
    return 0;
}

static ssize_t ipc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    size_t bytes_to_read;

    if (*f_pos >= buffer_len)
        return 0;

    bytes_to_read = min(count, buffer_len - *f_pos);

    if (copy_to_user(buf, shared_buffer + *f_pos, bytes_to_read)) {
        return -EFAULT;
    }

    *f_pos += bytes_to_read;
    printk(KERN_INFO "ipc_rw: Read %zu bytes\n", bytes_to_read);
    return bytes_to_read;
}

static ssize_t ipc_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    size_t bytes_to_write = min(count, (size_t)(SHARED_MEM_SIZE - 1));

    if (copy_from_user(shared_buffer, buf, bytes_to_write)) {
        return -EFAULT;
    }

    shared_buffer[bytes_to_write] = '\0';
    buffer_len = bytes_to_write;
    printk(KERN_INFO "ipc_rw: Wrote %zu bytes\n", bytes_to_write);
    return bytes_to_write;
}

static struct file_operations ipc_fops = {
    .owner = THIS_MODULE,
    .open = ipc_open,
    .release = ipc_release,
    .read = ipc_read,
    .write = ipc_write,
};

static int __init ipc_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &ipc_fops);
    if (major < 0) {
        pr_err("ipc_rw: Failed to register device\n");
        return major;
    }

    shared_buffer = kmalloc(SHARED_MEM_SIZE, GFP_KERNEL);
    if (!shared_buffer) {
        unregister_chrdev(major, DEVICE_NAME);
        return -ENOMEM;
    }

    buffer_len = 0;
    printk(KERN_INFO "ipc_rw: Device registered with major %d\n", major);
    return 0;
}

static void __exit ipc_exit(void) {
    kfree(shared_buffer);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "ipc_rw: Device unregistered\n");
}

module_init(ipc_init);
module_exit(ipc_exit);
