//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A Pthread condition variable demo program.
//
//
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx;
pthread_cond_t cvar;

void sender() {
  printf("Sender (tid:%ld) starts ...\n", pthread_self());

  // ... add code ...

  printf("Signal sent!\n");
}

void receiver() {
  printf("Receiver (tid:%ld) starts ...\n", pthread_self());

  // ... add code ...

  printf("Signal received!\n");
}

int main(int argc, char **argv) {
  pthread_t tid1, tid2;

  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&cvar, NULL);
  pthread_create(&tid2, NULL, (void *)receiver, NULL);
  pthread_create(&tid1, NULL, (void *)sender, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}
