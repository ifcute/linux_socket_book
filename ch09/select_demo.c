#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	// 读文件集合
	fd_set rd;
	// 时间间隔
	struct timeval tv;
	int err;

	FD_ZERO(&rd);
	FD_SET(0, &rd);

	// 5s超时时间
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	err = select(1, &rd, NULL, NULL, &tv);
	if (err == -1) {
		perror("select()");
	} else if(err) { 
		printf("Data is available now. \n");
	} else {
		printf("No data within five seconds. \n");
	}
	return 0;
}
