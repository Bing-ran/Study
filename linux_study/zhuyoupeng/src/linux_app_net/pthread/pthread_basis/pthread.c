#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


char buf[12] = {0};
pthread_t thread_t0, thread_t1;

void func1(void)
{
	int fd = -1;
	fd = open("/dev/input/mouse0",O_RDONLY);
	while(1){
		read(fd,buf,5);
		printf("***鼠标***：%s\n", buf);
	}
}

void func2(void)
{
	while(1){
		read(0,buf,5);
		printf("+++键盘+++：%s\n", buf);
	}
}

int main(int argc, char const *argv[])
{
	int ret = pthread_create(&thread_t0, NULL,(void *)func1, NULL);
	if (ret != 0)
	{
		printf("pthread_create thread_t0 error\n");
	}

	ret = pthread_create(&thread_t1, NULL,(void *)func2, NULL);
	if (ret != 0)
	{
		printf("pthread_create thread_t0 error\n");
	}

	while(1);

	return 0;
}