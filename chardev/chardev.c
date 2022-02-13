/*
 * chardev.c: Creates a read-only char device that says how many times you've
 * read from the dev file.
 *
 * You can have some fun with this by removing the module_get/put calls,
 * allowing the module to be removed while the file is still open.
 *
 * Compile with `make`. Load with `sudo insmod chardev.ko`. Check `dmesg | tail`
 * output to see the assigned device number and command to create a device file.
 *
 * From TLDP.org's LKMPG book.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h> /* for put_user */
#include <linux/time.h>

#include "chardev_ids.h"
#include "chardev_prototypes.h"


MODULE_LICENSE("GPL");

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;

static u32 times_openned_counter = 0;
static ktime_t ts_last_open = 0;
struct dentry *debug_file_open_counter = NULL;
struct dentry *debug_file_time_elapsed = NULL;

int my_open_counter_get(void *data, u64 *val)
{
    *val = (u64)times_openned_counter;
    printk("counter read from file %lld\n", *val);
    return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_open_counter, my_open_counter_get, NULL, "%llu\n");

int my_time_elapsed_get(void *data, u64 *val)
{
    *val = (u64)ktime_to_ns(ktime_sub(ktime_get(), ts_last_open)) / 1000000000;
    printk("time elapsed since last open %lld\n", *val);
    return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_time_elapsed, my_time_elapsed_get, NULL, "%llu\n");

static struct file_operations fops_unforgiving = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
  int ret_register;
  debug_file_open_counter = debugfs_create_file(OPEN_COUNTER_DEVICE_NAME, 0655, NULL, NULL, &fops_open_counter); 
  debug_file_time_elapsed = debugfs_create_file(TIME_ELAPSED_DEVICE_NAME, 0655, NULL, NULL, &fops_time_elapsed); 
  ret_register = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops_unforgiving);

  if (ret_register < 0) {
    printk(KERN_ALERT "Registering char device failed with %d\n", MAJOR_NUM);
    return ret_register;
  }

  printk(KERN_INFO "I was assigned major number %d. To talk to\n", MAJOR_NUM);
  printk(KERN_INFO "the driver, create a dev file with\n");
  printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, MAJOR_NUM);
  printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
  printk(KERN_INFO "the device file.\n");
  printk(KERN_INFO "Remove the device file and module when done.\n");

  return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
  /*
   * Unregister the device and remove debugfs files
   */
  unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
  debugfs_remove_recursive(debug_file_open_counter);
  debugfs_remove_recursive(debug_file_time_elapsed);
  printk(KERN_INFO "Bye Bye\n");
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *filp)
{

  if (Device_Open)
    return -EBUSY;

  Device_Open++;
  sprintf(msg, "I already told you %d times Hello world!\n", times_openned_counter++);
  msg_Ptr = msg;
  ts_last_open = ktime_get();
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *filp)
{
  Device_Open--;

  /*
   * Decrement the usage count, or else once you opened the file, you'll never
   * get rid of the module.
   *
   */
  module_put(THIS_MODULE);

  return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to read
 * from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */
                           char *buffer,      /* buffer to fill with data */
                           size_t length,     /* length of the buffer     */
                           loff_t *offset)
{
  /*
   * Number of bytes actually written to the buffer
   */
  int bytes_read = 0;

  /*
   * If we're at the end of the message, return 0 signifying end of file.
   */
  if (*msg_Ptr == 0)
    return 0;

  /*
   * Actually put the data into the buffer
   */
  while (length && *msg_Ptr) {
    /*
     * The buffer is in the user data segment, not the kernel segment so "*"
     * assignment won't work. We have to use put_user which copies data from the
     * kernel data segment to the user data segment.
     */
    put_user(*(msg_Ptr++), buffer++);
    length--;
    bytes_read++;
  }

  /*
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t
device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;
}

