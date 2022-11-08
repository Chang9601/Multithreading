#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1, mutex2;
int waste1 = 1000;
int waste2 = 1000;

void *
dumpToThread1(int waste) 
{
	/*
	 * 교착상태: 2개의 다른 프로세스/스레드가 서로에게 할당된 컴퓨터 자원을 얻기 위해 대기하면서 자신에게 할당된 컴퓨터 자원을 포기하지 않고 서로의 컴퓨터 자원을 무한대로 기다리는 상태
	 * 2개 이상의 스레드가 같은 집합의 뮤텍스를 잠그는 경우 교착상태 발생 가능
	 * thread1이 mutex1을 잠금 & mutex2를 잠그려고 시도 , thread2가 mutex2를 잠금 & mutex1을 잠그려고 시도 → 두 스레드 무기한 대기 (즉, 교착상태)
	 * 
	 * 해결책: 뮤텍스 계층 구조 필요 (즉, 스레드가 특정 순서로 뮤텍스를 항상 잠궈야 교착상태 예방)
	 * e.g., 뮤텍스 m1, m2가 있을 때 둘을 잠궈야 할 경우 m1을 먼저 잠그고 그 다음 m2
	 * 해제의 경우 스레드가 대기하는 것을 강제하기 않기 때문에 순서가 중요하지 않음
	 */
	 
	// 다음과 같이 순서가 일치하지 않을 경우 교착상태 발생 가능	 
	// pthread_mutex_lock(&mutex2);
	// pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);

	waste2 -= waste;
	waste1 += waste;

	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
}

void *
dumpToThread2(int waste)
{
	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);

	waste1 -= waste;
	waste2 += waste;

	pthread_mutex_unlock(&mutex1);
	pthread_mutex_unlock(&mutex2);
}

void *
loop1(void *arg)
{
	long n = *((long *)arg);
	for (int i = 0; i < n; i++) {
		dumpToThread2(1000);
	}
}

void *
loop2(void *arg)
{
	long n = *((long *)arg);
	for (int i = 0; i < n; i++) {
		dumpToThread1(1000);
	}
}

int
main(int argc, char *argv[])
{
	long n = 100000;
	pthread_t thread1, thread2;

	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);

	pthread_create(&thread1, NULL, (void *)loop1, (void *)&n);
	pthread_create(&thread2, NULL, (void *)loop2, (void *)&n);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	/* 더해지고 빼진 쓰레기 양이 동일하기 때문에 두 값은 초깃값과 일치 */
	printf("waste1: %d\n", waste1);
	printf("waste2: %d\n", waste2);

	return EXIT_SUCCESS;
}
