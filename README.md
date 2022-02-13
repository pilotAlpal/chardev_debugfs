# chardev_debugfs

> Linux module which
> - creates device file called “my_unforgiving_file” under /dev path. 
> - implements open system call for this file. 


> Module contains two counters, both are available through debugfs interface.
> - First counter keeps track of the number of times that “my_unforgiving_file” file was opened, available at /sys/kernel/debug/my_open_counter
> - The second counter keeps track of the time in seconds that elapsed since the file was opened for the last time. available at /sys/kernel/debug/my_last_open_elapsed.


> Short user space app, which calls open system calls on the my_unforgiving_file, is provided to test described module.

## References

<https://www.kernel.org/doc/html/latest/filesystems/debugfs.html>
<https://tldp.org/LDP/lkmpg/2.4/html/x856.html>
<https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch03s02.html>
<https://www.hitchhikersguidetolearning.com/2021/03/08/linux-kernel-adding-debugfs-support-to-a-linux-kernel-module/>

