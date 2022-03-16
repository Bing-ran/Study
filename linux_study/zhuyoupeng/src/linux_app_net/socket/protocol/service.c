#include "socket.h"



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
	seraddr.sin_port = htons(SERVICEPORT);		//服务器端口号
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

	info study = {
		.name = "清风",
		.age = 18,
	};

	info st;
	while(1){
		//第一步：服务器收
		ret = read(fd,&st,36);
		printf("name : %s\tage : %d\n", st.name,st.age);	

		//服务器解析数据，然后干活
		// if (buf[0] == GETNAME)
		// {
		// 	write(fd,study.name,strlen(study.name));
		// } else if (buf[0] == GETAGE)
		// {
		// 	write(fd,&study.age,1);
		// }

		//第三部：回复客户端
		// ret = send(fd,"ack",strlen("ack"),0);
		// if (ret < 0)
		// {
		// 	perror("send");
		// 	exit(-1);
		// }
	}



	return 0;
}