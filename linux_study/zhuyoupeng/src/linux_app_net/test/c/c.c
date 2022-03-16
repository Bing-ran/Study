#include <stdio.h>        
#include <sys/socket.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_SEN 	200
#define MAX_REC		200
#define SERADDR 	"192.168.204.147"  
#define SERPORT 	8000

int main(void)
{
	int c_fd = -1, num = 0;
	char cli_senbuf[MAX_SEN] = "abcdefghijklmn";  
	char cli_recbuf[MAX_REC];
	char dst[36] = {0};
	
	struct sockaddr_in sin = {0}; 
	struct sockaddr_in cin = {0}; 
	socklen_t addr_len;
	
	c_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( -1 == c_fd)
	{
		perror("socket");
		return -1;
	}
	printf("c_fd = %d.\n", c_fd);

	sin.sin_family = AF_INET;		
	sin.sin_port = htons(SERPORT);	
	inet_pton(AF_INET, SERADDR, &sin.sin_addr);

	num = sendto(c_fd, cli_senbuf, strlen(cli_senbuf), 0, (const struct sockaddr *)&sin, sizeof(sin));  
	printf("客户端成功发送了%d个字符\n", num);	
	printf("客户端发送的内容是：%s\n", cli_senbuf);

	
	num = recvfrom(c_fd, cli_recbuf, MAX_REC, 0, (struct sockaddr *)&cin, &addr_len);
	printf("客户端成功接收了%d个字节\n", num);
	printf("客户端接收的内容是:%s\n", cli_recbuf);
	

	inet_ntop(AF_INET,&cin.sin_addr.s_addr,dst,sizeof(dst));
	printf("ip : %s, port : %x\n", dst, cin.sin_port);
	
	return 0;
}