#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVICEADDR 	"192.168.204.147"
#define IPPORT 			6003
#define WAITMAXNUM 		100
int main(int argc, char const *argv[])
{
	int sockfd = 0, fd = 0, ret = -1;
	struct sockaddr_in seraddr = {0};
	struct sockaddr_in clientaddr = {0};
	socklen_t len = 0;

	//第一步：先socket打开文件描述符
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		_exit(-1);
	}
	printf("sockfd = %d\n", sockfd);

	//第二步：绑定本地端口及ip
	seraddr.sin_family = AF_INET;			//设置地址族为IPv4
	seraddr.sin_port = htons(IPPORT);		//服务器端口号
	seraddr.sin_addr.s_addr = inet_addr(SERVICEADDR);	//设置IP地址

	ret = bind(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
	if (ret  != 0)
	{
		perror("bind");
		exit(-1);
	}

	printf("bind success!\n");

	//第三步：listen监听端口
	ret = listen(sockfd,WAITMAXNUM);	//监听客户端来链接
	if (ret != 0)
	{
		perror("listen");
		exit(-1);
	}
	printf("listen ok!\n");
	//第四步：阻塞等待客户端来连接
	fd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
	if (fd < 0)
	{
		perror("accept");
		exit(-1);
	}

	printf("fd = %d\n", fd);

	char buf[24] = {0};
	while(1){
		ret = read(fd,buf,36);
		printf("this is service : %s\n", buf);	
		memset(buf,0,strlen(buf));
	}



	return 0;
}