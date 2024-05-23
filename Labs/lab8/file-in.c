//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A simple demo program of MPI file input.
//
// Usage: 
//   linux> mpirun -n <#procs> file-in <filename>
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
  int rank, buf[2]; 
  MPI_File fh; 
  MPI_Status st;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 2) {
    if (rank == 0)
      printf("Usage: mpirun -n <#procs> file-in <filename>\n");
    MPI_Finalize();
    return 0;
  }

  // all processes open and read from the same file
  MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  // read two integers
  MPI_File_read(fh, buf, 2, MPI_INT, &st);
  MPI_File_close(&fh);

  printf("rank=%d: buf=[%d,%d]\n", rank, buf[0], buf[1]);
  MPI_Finalize();
}
