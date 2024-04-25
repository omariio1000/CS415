//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Timing routines (C version).

#include "stdio.h"
#include "sys/time.h"   /* for gettimeofday,, clock_gettime */
#include <time.h>	/* for clock */
#include <unistd.h>	/* for sleep */

int main() {
  struct timeval start1, end1;
  struct timespec start2, end2, start3, end3;
  long start4, end4, sec, usec, nsec;
 
  gettimeofday(&start1, NULL);
  sleep(1);
  gettimeofday(&end1, NULL);

  clock_gettime(CLOCK_REALTIME, &start2);
  sleep(1);
  clock_gettime(CLOCK_REALTIME, &end2);

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start3); 
  sleep(1);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end3);

  start4 = clock(); 
  sleep(1);
  end4 = clock();

  sec = end1.tv_sec - start1.tv_sec;
  usec = end1.tv_usec - start1.tv_usec;
  printf("Elapsed time (gettimeofday):  sec=%ld, usec=%ld\n", sec, usec);

  sec = end2.tv_sec - start2.tv_sec;
  nsec = end2.tv_nsec - start2.tv_nsec;
  printf("Elapsed time (clock_gettime): sec=%ld, nsec=%ld\n", sec, nsec);

  sec = end3.tv_sec - start3.tv_sec;
  nsec = end3.tv_nsec - start3.tv_nsec;
  printf("CPU time (clock_gettime): sec=%ld, nsec=%ld\n", sec, nsec);

  usec = (end4 - start4) * 1000000 / CLOCKS_PER_SEC;
  printf("CPU time (clock): usec=%ld\n", usec);
}
