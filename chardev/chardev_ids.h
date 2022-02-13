/*  chardev_ids.h - the header file with module's ids
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the user apps reading from the device
 */

#ifndef CHARDEV_IDS_H
#define CHARDEV_IDS_H

#define SUCCESS 0
#define DEVICE_NAME "my_unforgiving_file"
#define OPEN_COUNTER_DEVICE_NAME "my_open_counter"
#define TIME_ELAPSED_DEVICE_NAME "my_last_open_elapsed"
#define BUF_LEN 80
/* The major device number. We can't rely on dynamic
 * registration any more, because
 * it is needed by user space example app
 * it. */
#define MAJOR_NUM 235



#endif
