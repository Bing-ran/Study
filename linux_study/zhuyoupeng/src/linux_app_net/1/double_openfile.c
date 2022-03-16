#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int fd1 = -1,fd2 = -1,ret = 0, i = 1;
	char buf[1024] = {0}, write_buf[] = "hello world!";

	char *filepath = "file.txt";

	/* 分别读文件 */
	fd1 = open(filepath, O_RDWR, 0666); 
	fd2 = open(filepath, O_RDWR, 0666); 

	if (fd1 < 0 || fd2 < 0)
	{
		printf("open failed!\n");
		_exit(-1);
	}

	while(i < 10){
		write(fd1,"ab",strlen("ab"));
		lseek(fd1,(i-1)*2,SEEK_SET);
		ret = read(fd1,buf,2);
		// if (ret > 0)
		// {
		 	printf("fd1 : %s\n", buf);
		// } else {
		// 	printf("read fd1 error!\n");
		// }
		memset(buf,0,sizeof(buf));

		write(fd2,"cd",strlen("cd"));
		lseek(fd2,(i-1)*2,SEEK_SET);
		ret = read(fd2,buf,2);
		// if (ret > 0)
		// {
		 	printf("fd2 : %s\n", buf);
		// } else {
		// 	printf("read fd2 error!\n");
		// }
		i++;
	}

	return 0;
}