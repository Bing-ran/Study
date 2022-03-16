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

#define PATH "/dev/buzzer"

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

int main(int argc, char *argv[])
{
    int fd = -1;

    fd = open(PATH, O_RDWR);
    if(fd < 0) {
        printf("open %s faled to %d\n", PATH, fd);
        return -1;
    }

    printf("open %s success, fd : %d\n", PATH, fd);

    ioctl(fd, PWM_IOCTL_SET_FREQ, 6000);
    sleep(3);
    ioctl(fd, PWM_IOCTL_STOP);
    sleep(3);
    ioctl(fd, PWM_IOCTL_SET_FREQ, 12000);
    sleep(3);
    ioctl(fd, PWM_IOCTL_STOP);

    // 关闭文件
    close(fd);

    return 0;
}