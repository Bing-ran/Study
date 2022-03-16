/*
 * @Author: your name
 * @Date: 2021-07-21 22:21:20
 * @LastEditTime: 2021-07-22 23:31:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \character_driverz:\linux_study\drivers\drivers\app.c
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define PATH "/dev/test"

int main(int argc, char *argv[])
{
    int fd = -1;
    char write_buf[] = "hello world!";
    char read_buf[16] = {0};
    fd = open(PATH, O_RDWR);
    if(fd < 0) {
        printf("open %s faled to %d\n", PATH, fd);
        return -1;
    }

    printf("open %s success, fd : %d\n", PATH, fd);

    // 读写操作
    write(fd, write_buf, sizeof(write_buf));
    read(fd, read_buf, sizeof(read_buf));
    printf("read : %s\n", read_buf);
    ioctl(fd, 1);

    sleep(10);
    ioctl(fd, 0);
    // 关闭文件
    close(fd);
}