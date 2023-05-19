// Filename: CSC1107_Group_Number_kernel.c

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "CSC1107device"
#define BUF_LEN 80

static int Major;
static char msg[BUF_LEN];
static char *msg_Ptr;

static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static int counter = 0;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

int init_module(void) {
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return 0;
}

void cleanup_module(void) {
    unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file) {
    if (counter) {
        return -EBUSY;
    }

    msg_Ptr = msg;
    try_module_get(THIS_MODULE);
    counter++;

    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    module_put(THIS_MODULE);
    counter--;
    return 0;
}


static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off) {
    if (len > sizeof(msg) - 1)
        len = sizeof(msg) - 1;

    strncpy(msg, buff, len);
    msg[len] = '\0';   // Ensure the message is null-terminated

    msg_Ptr = msg;
    return len;
}


static ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;

    if (*msg_Ptr == 0)
        return 0;

    while (len && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        len--;
        bytes_read++;
    }

    return bytes_read;
}


MODULE_LICENSE("GPL");
