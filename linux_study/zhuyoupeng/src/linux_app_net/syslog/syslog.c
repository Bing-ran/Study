#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	
	printf("my pid = %d.\n", getpid());

	openlog(NULL,LOG_PID | LOG_CONS, LOG_USER);

	syslog(LOG_INFO,"This is my loginfo");

	closelog();

	return 0;
}