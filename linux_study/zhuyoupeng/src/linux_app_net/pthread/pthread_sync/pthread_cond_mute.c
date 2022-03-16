#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

int flag = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
char buf[128] = {0};
pthread_t thread_t0;

void func1(void)
{
	while(flag == 0){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		printf("本次输入了 %ld.\n",strlen(buf));
		memset(buf,0,sizeof(buf));
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	printf("%s\n", "请输入一个字符串，以回车结束");


	int ret = pthread_create(&thread_t0, NULL,(void *)func1, NULL);

	while(1){
		scanf("%s",buf);
		pthread_cond_signal(&cond);
		if (!strncmp(buf,"end",3)){
			flag = 1;
			break;
		}
		

	}

	pthread_cond_destroy(&cond);
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