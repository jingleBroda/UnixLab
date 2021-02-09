#include <stdio.h>
#include <iostream>
#include <pthread.h>

pthread_cond_t pth_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t pth_lock = PTHREAD_MUTEX_INITIALIZER;
 
int ready = 0;

void provided(void *args)
{
	while(true) {
		pthread_mutex_lock(&pth_lock);
		if (ready == 1)
		{
			pthread_mutex_unlock(&pth_lock);
			continue;
		}
		ready = 1;
		printf("provided \n");
		pthread_cond_signal(&pth_cond);
		pthread_mutex_unlock(&pth_lock);
		sleep(1);
	}
}

void consumer(void *args)
{
	while(true) {
		pthread_mutex_lock(&pth_lock);
		while (ready == 0)
		{
			pthread_cond_wait(&pth_cond, &pth_lock);
		}
		ready = 0;
		printf("consumed \n");
		pthread_mutex_unlock(&pth_lock);
	}
}

int main(int argc, char **argv)
{
	pthread_t thr1;
	int arg1;
	pthread_t thr2;
	int arg2;

	pthread_create(&thr1, NULL, &provided, &arg1);
	pthread_create(&thr2, NULL, &consumer, &arg2);

	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
	return 0;
}
