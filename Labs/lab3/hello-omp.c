//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Hello world program (OMP version).
//
#include <stdio.h>
#include <omp.h>
#include <sched.h>

int main() {
   omp_set_num_threads(8);
#pragma omp parallel
  printf("Hello world! -- thread %d on core %d\n", omp_get_thread_num(), sched_getcpu());
}
