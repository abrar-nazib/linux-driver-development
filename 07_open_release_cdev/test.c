#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
    int fd;
    if(argc < 2){
        printf("I need the file to open as an argument\n");
        return 0;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        perror("Open");
        return fd;
    }
    printf("O_RDONLY SUCCESS\n");
    close(fd);

    fd = open(argv[1], O_RDWR | O_SYNC);
    if(fd < 0){
        perror("Open o_rdwr|o_sync");
        return fd;
    }
    printf("O_RDWR | O_SYNC SUCCESS\n");
    close(fd);

    fd = open(argv[1], O_WRONLY | O_NONBLOCK);
    if(fd < 0){
        perror("Open o_wronly|o_noblock");
        return fd;
    }
    printf("O_WRONLY | O_NONBLOCK SUCCESS\n");
    close(fd);

    return 0;

}