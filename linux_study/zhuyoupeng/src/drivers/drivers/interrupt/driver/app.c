#include <stdio.h>
#include  <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define KEY_PATH "/dev/input/event1"
#define MOUSE_PATH "/dev/input/event2"

#define X210_KEY "/dev/input/event1"

int main(int argc, char **argv)
{
    char buf_tmp[] = {1,2,3,4,5,6,7,8,9,10};
    char buf[64] = {0};
    int fd = -1, ret = -1, i = 0;
    int flag = 100;
    struct input_event in_event;

#if 1
    // 第一步：open设备
    fd = open(X210_KEY, O_RDONLY);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    // 第二步：读取输入事件
    while(flag--) {
        // 初始化in_event结构体
        memset(&in_event, 0, sizeof(in_event));
        ret = read(fd, &in_event, sizeof(struct input_event));
        if(fd < 0) {
            close(fd);
            perror("read");
            return -1;
        }
        printf("---------------------------------------------------------------------------------------------\n");
        printf("type:%d, code:%d, value:%d\n", in_event.type, in_event.code, in_event.value);
        printf("\n\n\n");
        usleep(1000);
    }

    // 第三步：close
    close(fd);

#else 
    for(i = 0; i < strlen(buf_tmp); i++) {
        sprintf(&buf[i * 4], "0x%x ", buf_tmp[i]);
    }

    printf(" +++++++++++ : [%s]\n", buf);
#endif
    return 0;
}