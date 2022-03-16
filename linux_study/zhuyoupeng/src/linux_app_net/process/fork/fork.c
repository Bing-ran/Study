#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	pid_t pid = -1;
	int ret = -1;
#if 0
	int fd = open("test.txt",O_RDWR | O_TRUNC);
	if (fd < 0)
	{
		perror("open");
		exit(-1);
	}
#endif
	pid = fork();
	if(	pid == 0){
		int fd = open("test.txt",O_RDWR);
		printf("子进程 n");
		ret = write(fd,"hello",5);

	} else if(pid > 0){
		printf("父进程\n");
		int fd = open("test.txt",O_RDWR);
		ret = write(fd,"world",5);

	} else {
		perror("fork");
		exit(-1);
	}

	
	return 0;
}