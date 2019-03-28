#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int ret;

	printf("系统分配的进程号: %d\n", getpid());
	ret = system("ping baidu.com -c 2");
	printf("返回值: %d\n", ret);
	return 0;


}
