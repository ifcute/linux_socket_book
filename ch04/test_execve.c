#include <stdio.h>
#include <unistd.h>

int main(int argc, char*argv[]) {
	char *args[] = {"/bin/ls", NULL};
	printf("系统分配的进程号是: %d\n", getpid());
	if (execve("/bin/ls", args, NULL) < 0) {
		printf("创建的进程出错\n");
	}
	return 0;
}
