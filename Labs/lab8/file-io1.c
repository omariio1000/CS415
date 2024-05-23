#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3)
    {
        if (rank == 0)
            printf("Usage: %s <inputfile> <outputfile>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    MPI_File fin, fout;
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fin);

    MPI_Offset fsize;
    MPI_File_get_size(fin, &fsize);
    int N = (int)fsize / sizeof(int);

    if (N % size != 0)
    {
        if (rank == 0)
            printf("N (%d) is not evenly divisible by P (%d)\n", N, size);
        MPI_Finalize();
        return 1;
    }

    int localN = N / size;
    int buf[localN];
    MPI_File_read_at(fin, rank * localN * sizeof(int), buf, localN, MPI_INT, MPI_STATUS_IGNORE);

    int localSum = 0;
    for (int i = 0; i < localN; ++i)
    {
        localSum += buf[i];
    }
    printf("P%d: psum = %d\n", rank, localSum);

    int totalSum;
    MPI_Reduce(&localSum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("The result sum is %d\n", totalSum);
    }

    for (int i = 0; i < localN; ++i)
    {
        buf[i] *= 2;
    }

    MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &fout);
    MPI_File_write_at(fout, rank * localN * sizeof(int), buf, localN, MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&fin);
    MPI_File_close(&fout);
    MPI_Finalize();
    return 0;
}
