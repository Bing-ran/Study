#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int fd = -1,ret = 0;
	char buf[1024] = {0}, write_buf[] = "hello world!";
#if 0
	if (argc < 2)
	{
		printf("usage: %s filename\n", argv[0]);
		_exit(-1);
	}
	char *filepath = argv[1];
#else
	char *filepath = "file.txt";
#endif
/*

	O_CRETE 	文件  存在：删除重建
				文件不存在：创建文件
	O_EXCL		和crete同时使用，文件存在则返回error
	
	O_APPEND	在文件末尾追加信息

	O_TRUNC		清除文件再往里写信息

*/
	fd = open(filepath, O_RDWR, 0666); //O_APPEND 接后面  O_TRUNC  open后文件为空
	if(fd < 0){
		printf("open %s failed!\n", "file.txt");
		perror("open");
		//errno();
		_exit(-1);
	} else {
		printf("%d\n",fd);
	}

#if 1
	ret = lseek(fd, 10, SEEK_SET);	//文件指针移动到文件末尾了
	ret = write(fd,write_buf,strlen(write_buf));
	if (ret < 0)
	{
		printf("%s\n", "write file failed!");
		_exit(-1);
	}
/*
	write read 都是从当前位置开始的
*/
	ret = lseek(fd, 0, SEEK_SET);	//文件指针移动到文件末尾了
	if (ret > 0)
	{
		printf("%d\n", ret);
	}
	ret = read(fd,buf,sizeof(buf));
	if(ret < 0){
		printf("%s\n", "read file failed!");
		_exit(-1);
	}
	printf("%d\t%s\n", ret, buf);
#else
	ret = lseek(fd,0,SEEK_END);
	if (ret > 0)
	{
		printf("%d\n", ret);
	}
#endif
	close(fd);
	return 0;
}