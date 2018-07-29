// Yet another producer consumer
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

#define ITEM_BUFFER 10

int itemcount = ITEM_BUFFER;
pthread_mutex_t mutex;
pthread_cond_t cond_producer, cond_consumer;

void init() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_producer, NULL);
	pthread_cond_init(&cond_consumer, NULL);
}

void producer() {
	pthread_mutex_lock(&mutex);
	while(1) {
		if(itemcount == ITEM_BUFFER) {
			pthread_cond_wait(&cond_producer, &mutex);
		}
		else {
			pthread_mutex_unlock(&mutex);
			printf("Producing...\n");
			sleep(5);
			pthread_mutex_lock(&mutex);
			itemcount++;
			printf("itemcount: %d\n", itemcount);
			if(itemcount == 1) {
				pthread_mutex_unlock(&mutex);
				pthread_cond_signal(&cond_consumer);
			}
			pthread_mutex_unlock(&mutex);
		}
	}
}

void consumer() {
	pthread_mutex_lock(&mutex);
	while(1) {
		if(itemcount == 0) {
			pthread_cond_wait(&cond_consumer, &mutex);
		}
		else {
			pthread_mutex_unlock(&mutex);
			printf("Consuming...\n");
			sleep(1);
			pthread_mutex_lock(&mutex);
			itemcount--;
			printf("itemcount: %d\n", itemcount);
			if(itemcount == ITEM_BUFFER-1) {
				pthread_mutex_unlock(&mutex);
				pthread_cond_signal(&cond_producer);
			}
			pthread_mutex_unlock(&mutex);
		}
	}
}

int main() {
	init();
	pthread_t p1, c1;
	//TODO: Spawn multiple producers, consumers and test
	pthread_create(&p1, NULL, (void*)producer, NULL);
	pthread_create(&c1, NULL, (void*)consumer, NULL);
	pthread_join(p1, NULL);
	pthread_join(c1, NULL);

	return 0;
}
