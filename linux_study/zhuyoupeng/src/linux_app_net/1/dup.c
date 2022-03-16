#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "file.txt"
#define FDNEW	 1
int main(int argc, char const *argv[])
{
	int fd1 = -1,fd2 = 100 ,ret = -1;

	fd1 = open(FILENAME,O_RDWR,0777);
	if (fd1 < 0)
	{
		printf("open %s failed!\n", FILENAME);
		perror("open");
		_exit(-1);
	}

	printf("fd1 = %d\n", fd1);
	close(FDNEW);
#if 0
	fd2 = dup(fd1);
	// printf("fd1 = %d\tfd2 = %d\n", fd1, fd2);
	// ret = -1;
	// fd2 = -1;
#else	
	fd2 = dup2(fd1,1);
#endif
	printf("fd1 = %d\tret = %d\tfd2 = %d\n", fd1,ret,fd2);

	close(fd1);
//	close(fd2);
	return 0;
}