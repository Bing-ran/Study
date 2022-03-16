#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

int main(int argc, char const *argv[])
{
	fd_set rfds, nfds;
	int fd = -1, ret = -1;
	char buf[36] = {0};

	fd = open("/dev/input/mouse0",O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	FD_ZERO(&rfds);
	FD_SET(fd,&rfds);
	FD_SET(0,&rfds);

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	// read(fd,buf,5);
	// printf("%s\n", buf);

	while(1){
		ret = select(fd+1,&rfds,NULL,NULL,&timeout);
//		ret = poll();
		for (int i = 0; i < ret; ++i)
		{
			memset(buf,0,sizeof(buf));
			if(FD_ISSET(0,&rfds)){
				read(0,buf,5);
				printf("键盘：%s", buf);
			} else if(FD_ISSET(fd,&rfds)){
				read(fd,buf,0);
				printf("鼠标：%s\n", buf);
			}
			ret = 0;
		}
		
	}

	return 0;
}