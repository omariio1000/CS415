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

long compute(long i) { return sqrt(i*i); }

int main(int argc, char **argv) {
  long N = 0;
  struct timeval start, end;

  if (argc == 1)
    printf("Usage: ./sum N");
  else {
    N = strtol(argv[1], NULL, 10);
  }  

  long sum = 0;
  gettimeofday(&start, NULL);
  for (int i = 0; i < N; i++)
    sum += compute(i);
  gettimeofday(&end, NULL);


  printf("Sum of %ld sqrt(i*i) is  %ld\n", N, sum);
  printf("Seq-version elapsed time:  %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);
}  
