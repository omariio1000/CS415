#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main() {
    int rank, size, x, y, z;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    x = 1; y = 2; z = 3;

    if (rank == 0) {
        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y, 1, MPI_INT, 1, MPI_COMM_WORLD);
    } else if (rank == 1) {
        // x = 3; y = 4; z = 5;
        MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&z, 1, MPI_INT, 1, MPI_COMM_WORLD);
    } else {
        // x = 6; y = 7; z = 8;
        MPI_Bcast(&z, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Bcast(&x, 1, MPI_INT, 1, MPI_COMM_WORLD);
    }

    printf("P%d: %d, %d, %d\n", rank, x, y, z);
    MPI_Finalize();
    return 0;
}
