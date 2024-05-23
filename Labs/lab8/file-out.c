//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A simple demo program of MPI file output.
//
// Usage: 
//   linux> mpirun -n <#procs> file-out <filename>
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank, buf[4], cnt=4, i;
  char fname[20];
  MPI_File fh;
  MPI_Status st;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 2) {
    if (rank == 0)
      printf("Usage: mpirun -n <#procs> file-out <filename>\n");
    MPI_Finalize();
    return 0;
  }

  // prepare data
  for (i=0; i<cnt; i++) 
    buf[i] = rank*100 + i;
  // each process writes to a separate output file
  sprintf(fname, "%s.%d", argv[1], rank);
  MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_CREATE|MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
  // write four integers
  MPI_File_write(fh, buf, cnt, MPI_INT, &st);
  MPI_File_close(&fh);

  MPI_Finalize();
}
