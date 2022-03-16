#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void func(int sig){
	if (sig == SIGALRM)
	{
		printf("Hello !\t");
	}
	
}

int main(int argc, char const *argv[])
{
	unsigned int ret = -1;

	struct sigaction act = {0};
	act.sa_handler = func;

	sigaction(SIGALRM,&act,NULL);
	while(1){
	ret = alarm(2);
#if 0
	while(1);
#else
	pause();
#endif
	printf("%d s\n", 2);
	}
	return 0;
}