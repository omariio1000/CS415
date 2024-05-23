//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A demo program of MPI concurrent I/O, with file view settings.
//
// Usage: 
//   linux> mpirun -n <#procs> file-view <infile> <outfile>
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank, offset, buf[4]; 
  MPI_File fin, fout;
  MPI_Status st;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 3) {
    if (rank == 0)
      printf("Usage: mpirun -n <#procs> file-view <infile> <outfile>\n");
    MPI_Finalize();
    return 0;
  }

  // all processes open the same pair of files
  MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fin);
  MPI_File_open(MPI_COMM_WORLD, argv[2], 
		MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fout);

  // each reads two integers, from a specific position
  offset = rank * 2 * sizeof(int);  // skip rank*2 integers
  MPI_File_set_view(fin, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_read(fin, buf, 2, MPI_INT, &st);
  printf("rank=%d: buf=[%d,%d]\n", rank, buf[0], buf[1]);

  // each writes four integers, to a specific position
  buf[2] = buf[3] = rank;
  offset = rank * 4 * sizeof(int);  // skip rank*4 integers
  MPI_File_set_view(fout, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_write(fout, buf, 4, MPI_INT, &st);

  MPI_File_close(&fin);
  MPI_File_close(&fout);
  MPI_Finalize();
}
