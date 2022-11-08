#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

/* 공유 자원 */
long count;

void *
increment(void *arg)
{
	long n = *((long *)arg);
	/* 원자성(한 번에 하나의 스레드만 실행)이 보장되어야 하는 임계구역 */
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	for (int i = 0; i < n; i++) {
		int tmp = count;
		tmp++;
		count = tmp;
	}
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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

	pthread_create(&thread1, NULL, (void *)increment, (void *)&n);
	pthread_create(&thread2, NULL, (void *)increment, (void *)&n);
	pthread_create(&thread3, NULL, (void *)increment, (void *)&n);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	
	/* 임계구역에 원자성을 보장하는 매커니즘이 없기 때문에 프로그램 실행마다 값이 동일할 수 도 있고 다를 수 있음 */
	printf("%s! %ld %s %ld\n", isSame(n * 3, count) ? "Great" : "Wrong", n * 3, isSame(n * 3, count) ? "==" : "!=", count);

	return EXIT_SUCCESS;
}
