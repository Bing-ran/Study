#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PATHFILE "/home/strudy1/sambashare/linux_app_net/exec/hello"

int main(int argc, char const *argv[])
{
	int ret = 0;
	pid_t pid = -1;

	pid = fork();
	if (pid == 0)
	{
		//execl(PATHFILE,"-a","-la",NULL);
		// char * const arg[] = {"-a","-la",NULL};
		// ret = execv(PATHFILE, arg);
		//execlp("ls","-l","-a",NULL);
		char * const envp[] = {"AA=aaa","BB=bbb",NULL};
		execle(PATHFILE,"ls","-l","-a",NULL,envp);

		return 0;

	} else if(pid > 0){

		printf("%s\n", "父进程");

	} else {

		perror("fork");
		exit(-1);

	}


	return 0;
}