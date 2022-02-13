#!/bin/bash

sudo insmod chardev.ko 
sudo mknod /dev/my_unforgiving_file c 235 0
printf "time elapsed:"| sudo cat - /sys/kernel/debug/my_last_open_elapsed
printf "counter value:"|sudo cat - /sys/kernel/debug/my_open_counter
cat /dev/my_unforgiving_file
cat /dev/my_unforgiving_file
printf "counter value:"|sudo cat - /sys/kernel/debug/my_open_counter
sleep 1s
printf "time elapsed:"| sudo cat - /sys/kernel/debug/my_last_open_elapsed
sleep 2s
printf "time elapsed:"| sudo cat - /sys/kernel/debug/my_last_open_elapsed
sudo rm -rf /dev/my_unforgiving_file
sudo rmmod chardev

