#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	int fd = -1, ret = 0;
	int pipefd[2] = {0};
	char buf[128] = {0};

	ret = pipe(pipefd);
	if (0 != ret)
	{
		perror("pipe");
		exit(-1);
	}

	fd = fork();
	if (fd == 0)
	{
		printf("子进程！\n");

		ret = read(pipefd[0],buf,sizeof(buf));
		if (ret > 0)
		{
			printf("子进程 rec：%s\n", buf);
			write(pipefd[1],"hello father!",strlen("hello father!"));
		} else {
			perror("read");
			exit(-1);
		}
		
	} else if(fd > 0){
		printf("父进程！\n");
		ret = write(pipefd[1],"hello son!",strlen("hello son!"));
		sleep(1);
		ret = read(pipefd[0],buf,sizeof(buf));
		printf("this is father rec: %s\n", buf);
	}

	return 0;
}

