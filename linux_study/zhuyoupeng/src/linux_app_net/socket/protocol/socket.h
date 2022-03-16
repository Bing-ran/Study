#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVICEADDR 	"192.168.204.147"
#define SERVICEPORT 	6006


#define GETNAME		1001
#define GETAGE		1002
typedef struct study
{
	char name[12] ;		//姓名
	int age;			//年龄
	int cmd;			//命令码
}info;


#endif