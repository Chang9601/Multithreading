#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *
printSentence(const char *arg)
{
	for (int i = 0; i < 5; i++) {
		printf("%s\n", arg);
	}
}

/* 모든 프로세스는 기본적으로 1개의 스레드 (즉, 메인 스레드) 소유 */
int
main(int argc, char *argv[])
{
	pthread_t thread1, thread2, thread3;
	
	/*
	 * 3개의 스레드 생성
	 * pthread_create() 함수 호출 이후, 어느 스레드가 CPU를 점유하는지에 대한 보장이 없음. 따라서 출력 결과는 프로그램 실행마다 변경될 수 있음
	 */
	pthread_create(&thread1, NULL, (void *)printSentence, (void *)"I'm so bored right now.");
	pthread_create(&thread2, NULL, (void *)printSentence, (void *)"Sorry. I kind of zoned out.");
	pthread_create(&thread3, NULL, (void *)printSentence, (void *)"I need to figure out something.");

	/*
	 * pthread_join() 함수는 매개변수로 들어온 스레드가 종료될 때까지 대기
	 * 프로세스에 실행되는 waitpid()와 몇 가지 차이점 존재
	 * 만약 pthread_join() 함수를 사용하지 않으면 메인 스레드가 바로 종료되기 때문에 출력값이 없음
	 */
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	return EXIT_SUCCESS;
}
