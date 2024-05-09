//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A matrix multiplication program (sequential version)  
//
// Usage: linux> ./mm [N]
//
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int N = 8;  // default array size

  if (argc > 1 && (N=atoi(argv[1])) < 1)
    N = 8;   

  int a[N][N], b[N][N], c[N][N];

  printf ("Matrix-Mult (N=%d):\n", N);

  // initialize matrices a and b
  for (int i=0; i<N; i++) 
    for (int j=0; j<N; j++) {
      a[i][j] = i + j + 2;
      b[i][j] = j + 1;
    }

  // multiplication
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++) {
      c[i][j] = 0;
      for (int k=0; k<N; k++)
	c[i][j] += a[i][k] * b[k][j];
    }

  // validation (sum-check)
  int total = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      total += c[i][j];
    }
  }
  printf("total = %d (N=%d)\n", total, N);
}
