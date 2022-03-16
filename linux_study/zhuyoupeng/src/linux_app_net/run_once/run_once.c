#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE "./RanBing"


void delate(void);

int main(int argc, char const *argv[])
{
	
	int fd = -1,i = 10;

	fd = open(FILE,O_RDWR | O_CREAT | O_EXCL,0666);
	if (fd < 0)
	{
		printf("Process already running,Please don't running %s again!\n", argv[0]);
		exit(0);
	}
	while(i--){
		printf("%s\n", "Hello World!");
		sleep(1);
	}

//	execl("/bin/rm","rm","RanBing",NULL);
//	system("rm RanBing");

	atexit(delate);	//注册进程结束清理函数
	return 0;
}


void delate(void)
{
	remove(FILE);
}