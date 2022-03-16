#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//#define VERSION
#ifdef VERSION
#define FILEPATH "/proc/version"
#else
#define FILEPATH "/proc/filesystems"
#endif

int main(int argc, char const *argv[])
{
	int fd = -1,ret = -1;
	char buf[256] = {0};

	fd = open(FILEPATH,O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		_exit(-1);
	}
	ret = read(fd,buf,sizeof(buf));
	if (ret < 0)
	{
		perror("read");
		_exit(-1);
	}
	printf("%s\n", buf);

	return 0;
}