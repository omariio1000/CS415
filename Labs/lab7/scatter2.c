//-----------------------------------------------------------------------------
// Program code for CS 415/515 Parallel Programming, Portland State University.
//-----------------------------------------------------------------------------

// Data array distrition.
//
//  Version 1: Works when P(#processes) evenly divides array size N.
//
//
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int N, P, rank, *data, *cnt, *disp;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  if (rank == 0)
  {
    printf("Enter the array size: ");
    fflush(stdout);
    scanf("%d", &N);
    data = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
      data[i] = i + 1;

    cnt = malloc(P * sizeof(int));
    disp = malloc(P * sizeof(int));

    int base = N / P;
    int remain = N % P;

    for (int i = 0; i < P; i++)
    {
      cnt[i] = base + (i < remain ? 1 : 0);
      disp[i] = (i == 0) ? 0 : disp[i - 1] + cnt[i - 1];
    }
  }
  else
  {
    cnt = malloc(P * sizeof(int));
    disp = malloc(P * sizeof(int));
  }

  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(cnt, P, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(disp, P, MPI_INT, 0, MPI_COMM_WORLD);

  int bsize = cnt[rank];
  int a[bsize];

  MPI_Scatterv(data, cnt, disp, MPI_INT, a, bsize, MPI_INT, 0, MPI_COMM_WORLD);

  printf("p%d block = [%d,%d] (size=%d), a = [%d,%d,...]\n",
         rank, disp[rank], disp[rank] + bsize - 1, bsize, a[0], a[1]);

  int psum = 0;
  for (int i = 0; i < bsize; i++)
    psum += a[i];

  int sum = 0;
  MPI_Reduce(&psum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    printf("Sum of data = %d (ref:%d)\n", sum, N * (N + 1) / 2);

  MPI_Finalize();
}
