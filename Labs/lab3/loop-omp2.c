//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A simple double loop (OMP version 2).
//
#include <stdio.h>
#include <omp.h>

#define N 4

int main() {
  int total=0;

  for (int i = 0; i < N; i++) {
#pragma omp parallel for
    for (int j = 0; j < N; j++) {
      total += i + j;
      printf("[%d,%d]\n", i, j);
    }
  }
  printf("Total = %d (should be 48)\n", total);

}
