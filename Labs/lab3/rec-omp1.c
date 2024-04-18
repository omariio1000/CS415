//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A simple recursive routine (OMP version 1).
//
#include <stdio.h>
#include <omp.h>

int array[8] = {1,2,3,4,5,6,7,8};

// array a[]'s size n is assumped to be a power of 2
//
void rec(int a[], int n) {
  if (n == 1) {
    printf("%d by thread %d\n", a[0], omp_get_thread_num());
  } else {
#pragma omp parallel sections
    {
#pragma omp section
    rec(a,n/2);
#pragma omp section
    rec(a+n/2, n/2);
    }
  }
}

int main() {
  rec(array, 8);
}
