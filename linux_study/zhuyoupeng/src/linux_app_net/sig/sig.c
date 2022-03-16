#include <stdio.h>
#include <signal.h>
#include <signal.h>

void sighand(int s);


int main(int argc, char const *argv[])
{
	struct sigaction sig = {0};
	sig.sa_handler = sighand;
#if 0
	signal(SIGINT, sighand);
	//signal(SIGINT, SIG_DFL);
#else
	sigaction(SIGINT,&sig,NULL);

#endif
	
	printf("%s\n", "Please ctrl+c");
	
	while(1){

	}

	return 0;
}

void sighand(int sig)
{
	if (SIGINT != sig)
	{
		printf("%s\n", "error");
	}
	printf("baibai! %d\n",sig);
}