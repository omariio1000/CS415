//-----------------------------------------------------------------------------
// Program code for CS 415/515 Parallel Programming, Portland State University.
//-----------------------------------------------------------------------------

// Block partition of a range [0..N-1].
//
//  Version 1: Works when P(#processes) evenly divides N.
//
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int N, P, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  if (rank == 0)
  {
    printf("Enter the array size: ");
    fflush(stdout);
    scanf("%d", &N);
  }
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int remain = N % P;
  int low = (N / P) * rank + (rank > remain ? remain : rank);
  int high = low + (N / P) + (rank < remain ? 1 : 0);
  int bsize = high - low;

  printf("p%d block = [%d,%d] (size=%d)\n", rank, low, high - 1, bsize);

  int total;
  MPI_Reduce(&bsize, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    printf("Total items = %d (N=%d)\n", total, N);

  MPI_Finalize();
}
