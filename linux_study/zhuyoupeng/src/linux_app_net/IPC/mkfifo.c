#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FILENAME "./test.txt"

int main(int argc, char const *argv[])
{
	int fd = -1, ret = -1;
	char buf[128] = {0};

	fd = fork();
	if (fd == 0)
	{
		ret = mkfifo("./test.txt",0777);
		fd = open(FILENAME,O_RDWR);
		ret = write(fd,"hello father!",strlen("hello father!"));
		if (ret <= 0)
		{
			perror("write");
			exit(-1);
		} 
		return 0;
	} else if (fd > 0)
	{
		ret = mkfifo("./test.txt",0777);
		fd = open(FILENAME,O_RDWR);
		read(fd,buf,sizeof(buf));
		printf("this is father : %s\n", buf);
	}

	return 0;
}