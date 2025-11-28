#ifndef MY_IOCTL_H_
#define MY_IOCTL_H_

struct mystruct {
    int repeat;
    char name[64];
};

#define WR_VAL _IOW('a', 'b', int*) // Writing data to kernel space
#define RD_VAL _IOR('a', 'b', int*) // Read data from kernel space
#define GREET _IOW('a', 'd', struct mystruct*)
#endif
