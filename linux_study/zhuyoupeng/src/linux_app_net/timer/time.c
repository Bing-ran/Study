#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char const *argv[])
{
	time_t *now = NULL;

	time_t timer = time(now);
	printf("%ld\n", timer);

	char *cnow = NULL;
	cnow = ctime(&timer);
	printf("%s\n", cnow);
	
	struct tm gtm;
	gmtime_r(&timer,&gtm);
	printf("gmtine:\nyear:%d\tmon:%d\tday:%d\thour:%d\tmin:%d\tseconds:%d\n",
			gtm.tm_year + 1900,gtm.tm_mon + 1,gtm.tm_mday,gtm.tm_hour,gtm.tm_min,gtm.tm_sec);

	localtime_r(&timer,&gtm);
	printf("localtime:\nyear:%d\tmon:%d\tday:%d\thour:%d\tmin:%d\tseconds:%d\n",
			gtm.tm_year + 1900,gtm.tm_mon + 1,gtm.tm_mday,gtm.tm_hour,gtm.tm_min,gtm.tm_sec);
	
	time_t timer_r = mktime(&gtm);
	printf("%ld\n", timer_r);

	char buf[36] = {0};
	asctime_r(&gtm,buf);
	printf("%s\n", buf);


	char s[36] = {0};
	size_t ret = strftime(s,sizeof(buf),"%F %T",&gtm);
	printf("%s\t%ld\n", s,ret);


	struct timeval tv = {0};
	struct timezone tz = {0};
	gettimeofday(&tv, &tz);
	printf("%ld\t%ld\n", tv.tv_sec,tv.tv_usec);
	printf("%d\t%d\n", tz.tz_minuteswest,tz.tz_dsttime);
	return 0;
}