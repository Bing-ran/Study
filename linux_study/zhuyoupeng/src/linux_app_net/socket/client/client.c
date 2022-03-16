#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVICEADDR 	"192.168.204.147"		//服务器开放出来的IP地址
#define SERVICEPORT 	6003

int main(int argc, char const *argv[])
{
	int sockfd = 0, fd = 0, ret = -1;
	struct sockaddr_in seraddr = {0};
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
	seraddr.sin_port = htons(SERVICEPORT);		//服务器端口号
	seraddr.sin_addr.s_addr = inet_addr(SERVICEADDR);	//设置IP地址
	ret = connect(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
	if (ret  != 0)
	{
		perror("connect");
		exit(-1);
	}

	printf("connect success!\n");

	char buf[36] = {0};
	while(1){
		printf("Please input some char!\n");
		scanf("%s",buf);

		write(sockfd, buf,strlen(buf));	
	}


	return 0;
}