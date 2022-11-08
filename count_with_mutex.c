#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

long count;
/* 뮤텍스: 원자성을 보장하는 매커니즘 */
pthread_mutex_t mutex;

void *
increment(void *arg)
{
	long n = *((long *)arg);
	/*
	 * 뮤텍스로 임계구역에 원자성 보장
	 * pthread_mutex_lock(): 뮤텍스 잠금
	 * pthread_mutex_unlock(): 뮤텍스 해제
	 */
	for (int i = 0; i < n; i++) {
		pthread_mutex_lock(&mutex);
		int tmp = count;
		tmp++;
		count = tmp;
		pthread_mutex_unlock(&mutex);
	}
}

bool
isSame(long a, long b)
{
	return a == b;
}

int
main(int argc, char *argv[])
{
	long n = 500000;
	pthread_t thread1, thread2, thread3;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&thread1, NULL, (void *)increment, (void *)&n);
	pthread_create(&thread2, NULL, (void *)increment, (void *)&n);
	pthread_create(&thread3, NULL, (void *)increment, (void *)&n);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	/* 뮤텍스로 임계구역에 원자성을 보장하기 때문에 항상 같은 값 */
	printf("%s! %ld %s %ld\n", isSame(n * 3, count) ? "Great" : "Wrong", n * 3, isSame(n * 3, count) ? "==" : "!=", count);

	return EXIT_SUCCESS;
}
