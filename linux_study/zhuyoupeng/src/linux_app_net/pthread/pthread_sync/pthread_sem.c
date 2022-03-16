#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <semaphore.h>

int flag = 0;
sem_t sem;
char buf[1024] = {0};
pthread_t thread_t0;

void func1(void)
{
	while(strncmp(buf,"end",3) != 0){
		
		if (strlen(buf) == 0)
		{
			continue;
		}
		printf("本次输入了 %ld.\n",strlen(buf));
		sem_wait(&sem);
		memset(buf,0,sizeof(buf));
	}

	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	

	sem_init(&sem,0,0);	 	//初始化信号量

	printf("%s\n", "请输入一个字符串，以回车结束");


	int ret = pthread_create(&thread_t0, NULL,(void *)func1, NULL);

	while(scanf("%s",buf)){
		if (strncmp(buf,"end",3) != 0)
		{
			sem_post(&sem);
		} else {
			printf("%s\n", "程序结束");
//			
			sem_post(&sem);
			break;
		}
		
	}
	sem_destroy(&sem);
	printf("%s\n", "wait 回收子线程\n");
	ret = pthread_join(thread_t0,NULL);
	if (ret != 0)
	{
		perror("pthread_join");
		exit(-1);
	}
	printf("%s\n", "子线程回收 ok");

	return 0;
}