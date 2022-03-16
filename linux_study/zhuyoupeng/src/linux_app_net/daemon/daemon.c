#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void create_daemon(void);

int main(int argc, char const *argv[])
{
	


	create_daemon();

	while(1){
		printf("%s\n", "I am running!");
		sleep(1);
	}

	return 0;
}


/*
*	funcion -- 将调用它的进程变成一个守护进程
*/
void create_daemon(void)
{
	pid_t pid = -1;

	pid = fork();
	if (pid > 0)
	{
		exit(0);
	} else {
		perror("fork");
	}

	//子进程就是这里
	pid = setsid();
	if (pid < 0)
	{
		perror("setsid");
		exit(-1);
	}

	chdir("/");

	umask(0);

	int count = sysconf(FOPEN_MAX);
	for (int i = 0; i < count; ++i)
	{
		close(i);
	}
	
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);
}