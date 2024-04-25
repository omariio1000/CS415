//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The sum program (Sequential version).
//
//
#include <stdio.h>
#include <math.h>

#define N 1000 	/* problem parameter */

long compute(long i) { return sqrt(i*i); }

int main(int argc, char **argv) {
  long sum = 0;
  for (int i = 0; i < N; i++)
    sum += compute(i);
  printf("Sum of %ld sqrt(i*i) is %ld\n", N, sum);
}  
