#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../chardev/chardev_ids.h"

#define DEV_PATH "/dev"
#define DEBUG_FS_PATH "/sys/kernel/debug"
#define MODULE_PATH "../chardev/chardev.ko"


void load_module()
{
    char *insmod_pref = "insmod ";
    char *insmod_cmd = (char *)malloc((strlen(insmod_pref) + strlen(MODULE_PATH) + 1) * sizeof(char));
    if (insmod_cmd != NULL)
    {
        strcpy(insmod_cmd, insmod_pref);
        strcpy(insmod_cmd + strlen(insmod_pref), MODULE_PATH);
        printf ("loading module\n");
        if (system (insmod_cmd) == 0)
        {
            printf ("Module loaded successfully\n");
        }
    }
}

void unload_module()
{
    char *rmmod_pref = "rmmod ";
    char *rmmod_cmd = (char *)malloc((strlen(rmmod_pref) + strlen(MODULE_PATH) + 1) * sizeof(char));
    if (rmmod_cmd != NULL)
    {
        strcpy(rmmod_cmd, rmmod_pref);
        strcpy(rmmod_cmd + strlen(rmmod_pref), MODULE_PATH);
        printf ("unloading module\n");
        if (system (rmmod_cmd) == 0)
        {
            printf ("Module unloaded successfully\n");
        }
    }
}

char *get_absolute_path(char *base_dir, char *filename)
{
    char *path = (char *)malloc((strlen(base_dir) + strlen(filename) + 2) * sizeof(char));
    strcpy(path, base_dir);
    strcpy(path  + strlen(base_dir), "/");
    strcpy(path + strlen(base_dir) + 1, filename);
    return path;
}

int main(int argc, char *argv[])
{
    int rc; 
    FILE *f_unforg, *f_time_elapsed, *f_open_counter;
    char *unforgiving_file_name, *open_counter_file_name, *last_open_elapsed_file_name; 
    char info[BUF_LEN];
    int value = -1;

    load_module();

    unforgiving_file_name = get_absolute_path(DEV_PATH, DEVICE_NAME);
    rc = mknod(unforgiving_file_name, S_IFCHR|0, makedev(MAJOR_NUM, 0));
    if (rc < 0) {
        printf ("mknod: Can't create file: %s\n", unforgiving_file_name);
        exit(-1);
    }
    f_unforg = fopen(unforgiving_file_name, "r");
    fgets(info, BUF_LEN, f_unforg);
    printf("Received message=%s\n",info);

    open_counter_file_name = get_absolute_path(DEBUG_FS_PATH, OPEN_COUNTER_DEVICE_NAME);
    f_open_counter = fopen(open_counter_file_name, "r");
    fscanf(f_open_counter,"%d", &value);
    printf("Counter=%d\n",value);

    sleep(8);
    last_open_elapsed_file_name = get_absolute_path(DEBUG_FS_PATH, TIME_ELAPSED_DEVICE_NAME);
    f_time_elapsed = fopen(last_open_elapsed_file_name, "r");
    fscanf(f_time_elapsed,"%d", &value);
    printf("Time Elapsed=%d\n",value);

    fclose(f_time_elapsed);
    fclose(f_open_counter);
    fclose(f_unforg);
    remove(unforgiving_file_name);

    unload_module();
    exit(SUCCESS);
}

