//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The sum program (Sequential version).
//
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <sched.h>

long compute(long i) { return sqrt(i*i); }

int main(int argc, char **argv) {
  long N = 0;
  int threads = 1;
  struct timeval start, end;

  if (argc == 1) {
    printf("Usage: ./sum N");
    return EXIT_FAILURE;
  }

  else if (argc == 3) 
    threads = strtod(argv[2], NULL);

  N = strtol(argv[1], NULL, 10);
  omp_set_num_threads(threads);

  long sum = 0;
  gettimeofday(&start, NULL);


#pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < N; i++) {
    sum += compute(i);
#ifdef DEBUG
  printf("Thread id: %d\n", omp_get_thread_num());
#endif
  }

  gettimeofday(&end, NULL);


  printf("Sum of %ld sqrt(i*i) is  %ld\n", N, sum);
  long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
  printf("OMP-version (%d threads) elapsed time:  %ld us\n", threads, elapsed_time);

}  
