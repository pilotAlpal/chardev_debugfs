/*  chardev_prorotypes.h - the header file with the device's prototypes
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the user apps reading from the device
 */

#ifndef CHARDEV_PROTOTYPES_H
#define CHARDEV_PROTOTYPES_H

/*
 * Prototypes
 */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#endif
