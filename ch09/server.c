#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

void process_conn_serv(int s);
extern void sig_process(int signo);
#define PORT 8888
#define BACKLOG 2

int main(int argc, char *argv[]) {
	// 服务器，客户端socket描述符
	int ss, sc;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;
	signal(SIGINT, sig_process);
	signal(SIGPIPE, sig_process);

	ss = socket(PF_INET, SOCK_STREAM, 0);
	if (ss < 0) {
		printf("socket err\n");
		return -1;
	}

	/* 设置服务器地址*/
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	err = bind(ss, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (err < 0) {
		printf("bind error\n");
		return -1;
	}
	err = listen(ss, BACKLOG);
	if (err  < 0) {
		printf("listen error \n");
		return -1;
	}

	for (;;) {
		int addrlen = sizeof(struct sockaddr);
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		if (sc < 0) {
			continue;
		}

		pid = fork();
		if (pid == 0) {
			close(ss);
			process_conn_serv(sc);
		} else {
			close(sc);
		}
	
	}

	
	return 0;
}

void process_conn_serv(int s) {
	ssize_t size = 0;
	char buffer[1024];

	for (;;) {
		size = recv(s, buffer, 1024, 0);
		if (size == 0) {
			return;
		}
		sprintf(buffer, "%d bytes altogether\n", size);
		send(s, buffer, strlen(buffer) + 1, 0);
	}
}

void sig_process(int signo) {

	printf("Catch a exit signal:%d\n", signo);
	_exit(0);
}
