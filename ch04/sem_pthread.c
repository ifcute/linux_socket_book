#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void *producer_f(void * arg);
void *consumer_f(void *arg);

sem_t sem;
int running = 1;
int main(void) {
	pthread_t producer_t, consumer_t;
	sem_init(&sem, 0, 16);

	pthread_create(&producer_t, NULL, (void*)producer_f, NULL);
	pthread_create(&consumer_t, NULL, (void*)consumer_f, NULL);

	sleep(1);
	running = 0;
	pthread_join(consumer_t, NULL);
	pthread_join(producer_t, NULL);
	sem_destroy(&sem);

	return 0;
}

void *producer_f(void *arg) {
	int semval = 0;
	while(running) {
		usleep(1);
		// 信号量+1
		sem_post(&sem);
		sem_getvalue(&sem, &semval);
		printf("生产，总数量:%d\n", semval);
	}
}

void *consumer_f(void *arg) {
	int semval = 0;

	while(running) {
		usleep(1);
		// 信号量 -1， 如果信号量为0，则阻塞，知道信号量大于0
		sem_wait(&sem);
		sem_getvalue(&sem, &semval);
		printf("消费，总数量：%d\n", semval);
	}
}
