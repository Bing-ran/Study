#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char const *argv[])
{
	int fd = -1 ,ret = -1;
	int flag = -1;
	char buf[10] = {0};
#if 0
	memset(buf,0,sizeof(buf));
	printf("%s\n", "will read!\n");
	ret = read(0,buf,sizeof(buf));
	if (ret > 0)
	{
		printf("键盘 [%s]\n", buf);
		ret = -1;
	}
	

	memset(buf,0,sizeof(buf));
	fd = open("/dev/input/mouse0",O_RDONLY);
	if (fd < 0)
	{
		perror("open");
	}
	ret = read(fd,buf,sizeof(buf)); 
	if (ret > 0)
	{
		printf("鼠标 [%s]\n", buf);
		ret = -1;
	}
#else 
	flag = fcntl(0, F_GETFL);
	flag |= O_NONBLOCK;

	fcntl(0, F_SETFL);
	fd = open("/dev/input/mouse0",O_RDONLY);
	while(1){

		ret = read(0,buf,5);
		if (ret > 0)
		{
			printf("键盘：%s\n", buf);
			memset(buf,0,sizeof(buf));
			ret = -1;
		}
		
		ret = read(fd,buf,5); 
		if (ret > 0)
		{
			printf("鼠标：%s\n", buf);
			memset(buf,0,sizeof(buf));
			ret = -1;
		}

	}

#endif
	return 0;
}