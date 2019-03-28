#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

extern void sig_process(int signo);
extern void sig_pipe(int signo);

static int s;
// 客户端信号处理回调函数
void sig_process(int signo) {
	printf("Catch a exit signal\n");
	close(s);
	exit(0);
}
void sig_pipe(int signo) {
	printf("Catch a SIGPIPE signal :%d\n", signo);
	//释放资源
	close(s);
	exit(0);
}

void process_conn_client(int s) {
	ssize_t size = 0;
	char buffer[1024];

	for (;;) {
		size = read(0, buffer, 1024);
		if (size > 0) {
			send(s, buffer, size, 0);
			size = recv(s, buffer, 1024, 0);
			write(1, buffer, size);
		}
	}
}

#define PORT 8888

int main(int argc, char *argv[]) {
	struct sockaddr_in serv_addr;
	int err;

	if(argc == 1) {
		printf("PLS input server addr\n");
		return 0;
	}

	signal(SIGINT, sig_process);
	signal(SIGPIPE, sig_pipe);

	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printf("socket error\n");
		return -1;
	}

	/* 设置服务器地址*/
	// 将地址结构清0
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	// 连接服务器
	connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	// 客户端处理过程
	process_conn_client(s);
	close(s);

	return 0;
}

