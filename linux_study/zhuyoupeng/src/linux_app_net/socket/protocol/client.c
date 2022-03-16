#include "socket.h"


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

	int sendbuf[36] = {0};
	char recvbuf[36] = {0};
	info st;
	while(1){
		//第一步：客户端发送数据给服务器
		printf("Please input study name\n");
		scanf("%s",st.name);
		printf("Please input study age\n");
		scanf("%d",&st.age);
		write(sockfd, &st,sizeof(st));	

		//第二步：接收服务器的相应
		// memset(recvbuf,0,sizeof(recvbuf));
		// ret = recv(sockfd,&recvbuf,3,0);
		// if (ret <0)
		// {
		// 	perror("recv");
		// 	exit(-1);
		// }
		// printf("Recv action : %s\n", recvbuf);

		//第三步：客户端解析服务器的回复，再做下一步的操作

	}


	return 0;
}