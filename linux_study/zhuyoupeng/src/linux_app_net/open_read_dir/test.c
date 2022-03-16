#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


#define PATHNAME "/home/strudy1/sambashare/linux_app_net/open_read_dir"

int main(int argc, char const *argv[])
{
	int ret = -1,cnt = 0;
	DIR *dir = NULL;
	struct dirent *dirrent = NULL;

	if (argc != 2)
	{
		printf("usage: %s dirname\n", argv[0]);
		return -1;
	}
	dir = opendir(argv[1]);
	if(dir == NULL){
		perror("opendir");
		return -1;
	}
#if 1
	while(1){
		dirrent = readdir(dir);
		if (dirrent != NULL)
		{
			cnt++;
			
			if (dirrent->d_type == DT_REG)
			{
				printf("type = %d\tname = %s\n",dirrent->d_type,dirrent->d_name);
			} else {
				printf("type = %d\tname = %s\n",dirrent->d_type,dirrent->d_name);
			}
		} else {
			printf("总文件数：%d\n", cnt);
			return -1;
		}
	}
#else
	dirrent = readdir(dir);
	dirrent = readdir(dir);
	printf("type = %c\tname = %s\n",dirrent->d_type,dirrent->d_name);
#endif
	printf("%d\n",cnt);

	return 0;
}