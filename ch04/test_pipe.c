#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	int result = -1;
	int fd[2];
	pid_t pid;
	char string[] = "你好管道";
	char readbuffer[80];
	/* 文件描述符1用于写，文件描述符0用于读*/
	int *write_fd = &fd[1];
	int *read_fd = &fd[0];
	result = pipe(fd);
	if (-1 == result) {
		printf("创建管道失败");
		return -1;
	}
	pid = fork();
	if (-1 == pid) {
		printf("fork进程失败\n");
		return -1;
	}

	if (0 == pid) {
		close(*read_fd);
		result = write(*write_fd, string, sizeof(string));
		return 0;
	} else {
		close(*write_fd);
		result = read(*read_fd, readbuffer, sizeof(readbuffer));
		printf("接收到%d个数据，内容为:%s\n", result, readbuffer);
	}

	return 0;
}
