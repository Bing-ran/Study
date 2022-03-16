#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


#define MAX_SEN 	200
#define MAX_REC		200
#define SERPORT 	8000

int main(void)
{
	int s_fd = -1, ret = -1;
	char ser_senbuf[MAX_SEN] = "hello"; 
	char cli_recbuf[MAX_REC]; 
	
	struct sockaddr_in ser_sin = {0};  
	struct sockaddr_in ser_cin = {0};   
	socklen_t addr_len;
    char ipbuf[INET_ADDRSTRLEN];
	
	s_fd = socket(AF_INET,SOCK_DGRAM,0);
	if (s_fd == -1 )
	{
		perror("socket");
		return -1;
	}
	printf("s_fd = %d.\n", s_fd);
		
	bzero(&ser_sin,sizeof(ser_sin)); 
	ser_sin.sin_family = AF_INET;		
	ser_sin.sin_addr.s_addr = INADDR_ANY;
	ser_sin.sin_port = htons(SERPORT);	
	ret = bind(s_fd, (const struct sockaddr *)&ser_sin, sizeof(ser_sin));  
	if (ret < 0)
	{
		perror("bind");
		return -1;
	}
	printf("bind success.\n");

    addr_len = sizeof(ser_cin);
    ret = recvfrom(s_fd, cli_recbuf, MAX_REC, 0, (struct sockaddr *)&ser_cin, &addr_len);
    printf("服务器成功接收了%d个字节\n", ret);
	printf("服务器接收的内容是：%s\n", cli_recbuf);
    
	inet_ntop(AF_INET, &ser_cin.sin_addr, ipbuf,sizeof(ipbuf));
	printf("ip地址是：%s\n", ipbuf);
		
	ret = sendto(s_fd, ser_senbuf, strlen(ser_senbuf), 0, (const struct sockaddr *)&ser_cin, addr_len); 
	printf("服务器成功发送了%d个字符\n", ret);	
	printf("服务器发送的内容是：%s\n", ser_senbuf);
 
	return 0;
}