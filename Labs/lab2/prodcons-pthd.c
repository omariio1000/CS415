//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A Pthread producer-consumer program.
// 
//
#include <stdio.h>
#include <pthread.h>

#define BUFSIZE  20 
#define NUMITEMS 100

int buffer[BUFSIZE];
int idx = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void producer() { 
  printf("Producer starting\n"); 
  for (int i = 1; i <= NUMITEMS; i++) {
    pthread_mutex_lock(&lock);
    while (idx == BUFSIZE)
      pthread_cond_wait(&notFull, &lock);
    buffer[idx++] = i;
    printf("Producer added %d (bsz: %d)\n", i, idx); 
    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&lock);
  }
  printf("Producer ending\n"); 
} 

void consumer() { 
  printf("Consumer starting\n");
  for (int i = 1; i <= NUMITEMS; i++) {
    pthread_mutex_lock(&lock);
    while (idx == 0)
      pthread_cond_wait(&notEmpty, &lock);
    int val = buffer[--idx];
    printf("Consumer rem'd %d (bsz: %d)\n", val, idx); 
    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&lock);
  }
  printf("Consumer ending\n"); 
} 

int main(int argc, char **argv) {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, (void*)producer, NULL);
  pthread_create(&tid2, NULL, (void*)consumer, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("Main: all done!\n"); 
}
