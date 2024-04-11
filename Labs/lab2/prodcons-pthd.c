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

void producer() { 
  printf("Producer starting\n"); 
  for (int i = 1; i <= NUMITEMS; i++) {
    buffer[idx++] = i;
    printf("Producer added %d (bsz: %d)\n", i, idx); 
  }
  printf("Producer ending\n"); 
} 

void consumer() { 
  printf("Consumer starting\n");
  for (int i = 1; i <= NUMITEMS; i++) {
    int val = buffer[--idx];
    printf("Consumer rem'd %d (bsz: %d)\n", val, idx); 
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
