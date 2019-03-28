#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define K 1024
#define WRITELEN ( K * 128)

int main(int argc, char *argv[]) {
	int result = -1;
	int fd[2], nbytes;
	pid_t pid;
	char string[WRITELEN] = "你好管道";
	char readbuffer[10*K];
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
		int write_size = WRITELEN;
		result = 0;
		while(write_size >= 0) {
			result = write(*write_fd, string, write_size);
			if (result > 0) {
				write_size -= result;
				printf("写入%d个数据,剩余%d个数据\n",result, write_size);
			} else {
				sleep(10);
			}
		}
		return 0;
	} else {
		close(*write_fd);
		while (1) {
			nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
			if (nbytes <= 0) {
				printf("没有数据类\n");
				break;
			}
		
			printf("接收到%d个数据，内容为:%s\n", nbytes, readbuffer);
		}
	}

	return 0;
}
