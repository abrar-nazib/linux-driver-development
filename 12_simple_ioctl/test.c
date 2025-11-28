#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h> 

int main(){
    int fd = open("/dev/simple_ioctl", O_RDWR);

    if(fd < 0){
        perror("Open error!");
        return fd;
    }

    char c = 123;
    int i = 5678, status;

    status = ioctl(fd, 1, NULL);
    printf("ioctl returned %d\n", status);

    status = ioctl(fd, 1234, &c);
    printf("ioctl returned %d\n", status);

    status = ioctl(fd, 987654321, &i);
    printf("ioctl returned: %d\n", status);

    close(fd);

    return 0;
}