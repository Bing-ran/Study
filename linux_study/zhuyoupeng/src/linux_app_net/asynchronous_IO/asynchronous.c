#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int fd = 0;
char buf[24] = {0};

void func(int sig)
{
	if (sig == SIGIO)
	{
		memset(buf,0,sizeof(buf));
		read(fd,buf,5);
		printf("-------------：%s\n", buf);

	}
}

int main(int argc, char const *argv[])
{
	int flag = -1;

	fd = open("/dev/input/mouse0",O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	//鼠标文件描述符可以接受异步通知
	flag = fcntl(fd,F_GETFL);
	flag |= O_ASYNC;
	fcntl(fd,F_SETFL,flag);

	//set异步进程为当前进程
	fcntl(fd,F_SETOWN,getpid());

	//注册当前进程异步IO捕获函数
	signal(SIGIO,func);

	while(1){
		read(0,buf,5);
		printf("+++++++：%s", buf);	
		memset(buf,0,sizeof(buf));
	}

	return 0;
}