#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pid_t pid = -1,id = -1;
	int status = -1;

	pid = fork();
	if(pid == 0){
		printf("子进程 pid = %d\n", getpid());
		sleep(2);

	} else if(pid > 0){

		printf("%s\n", "父进程");
#if 0
		id = wait(&status);
#else
		id = waitpid(-1, &status, 0);
#endif
		printf("wait return = %d\n", id);

	}
	
	printf("%s\n", "over!");

	return 0;
}