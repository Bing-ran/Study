#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPADDR "192.168.10.1"
//1		0a 		a8 		c0
//1		10		168		192
//网络字节序，其实就是大端模式

int main(int argc, char const *argv[])
{
//	in_addr_t addr = 0;
//	struct in_addr inet_pton_addr;
	const char *inet_ntop_addr = NULL;
	char dst[16] = {0};
	struct in_addr addr;
	addr.s_addr = 0x10aa8c0;
	//addr = inet_addr(IPADDR);
	//int ret = inet_pton(AF_INET, IPADDR, &inet_pton_addr);
	inet_ntop_addr = inet_ntop(AF_INET, &addr, dst, sizeof(dst));


	printf("%s\n", dst);

	return 0;
}