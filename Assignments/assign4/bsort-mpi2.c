#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Check if x is a power of 2
#define IsPowerOf2(x) (!((x) & ((x) - 1)))

// Bucket index for integer x of b bits (B is #buckets)
#define BktIdx(x, b, B) ((x) >> ((b) - (int)log2(B)))

// Bubble sort
void bubble_sort(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (a[i] > a[j])
            {
                int tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3)
    {
        if (rank == 0)
        {
            printf("Usage: mpirun -n P ./bsort-mpi2 <infile> <outfile>\n");
        }
        MPI_Finalize();
        exit(0);
    }

    if (!IsPowerOf2(size))
    {
        if (rank == 0)
        {
            printf("Number of processes P must be a power of 2\n");
        }
        MPI_Finalize();
        exit(0);
    }

    FILE *fin = fopen(argv[1], "rb");
    if (fin == NULL)
    {
        if (rank == 0)
        {
            perror("Error opening input file");
        }
        MPI_Finalize();
        exit(1);
    }

    fseek(fin, 0L, SEEK_END);
    int size_bytes = ftell(fin);
    rewind(fin);

    int N = size_bytes / sizeof(int);
    int min_local_N = N / size;
    int remainder = N % size;
    int local_N = (rank < remainder) ? min_local_N + 1 : min_local_N;

    int *local_data = (int *)malloc(local_N * sizeof(int));
    if (local_data == NULL)
    {
        perror("Error allocating memory");
        fclose(fin);
        MPI_Finalize();
        exit(1);
    }

    int offset = (rank < remainder) ? rank * (min_local_N + 1) : rank * min_local_N + remainder;
    fseek(fin, offset * sizeof(int), SEEK_SET);
    size_t read_count = fread(local_data, sizeof(int), local_N, fin);
    if (read_count != local_N)
    {
        if (rank == 0)
        {
            printf("Error reading input file: expected %d elements, got %zu elements\n", local_N, read_count);
        }
        free(local_data);
        fclose(fin);
        MPI_Finalize();
        exit(1);
    }
    fclose(fin);

    int num_buckets = size;
    int **buckets = (int **)malloc(num_buckets * sizeof(int *));
    int *bucket_sizes = (int *)malloc(num_buckets * sizeof(int));
    for (int k = 0; k < num_buckets; k++)
    {
        buckets[k] = (int *)malloc(2 * local_N * sizeof(int));
        bucket_sizes[k] = 0;
    }

    for (int i = 0; i < local_N; i++)
    {
        int k = BktIdx(local_data[i], N, num_buckets);
        buckets[k][bucket_sizes[k]++] = local_data[i];
    }

    int *recv_counts = (int *)malloc(size * sizeof(int));
    MPI_Alltoall(bucket_sizes, 1, MPI_INT, recv_counts, 1, MPI_INT, MPI_COMM_WORLD);

    int *recv_displs = (int *)malloc(size * sizeof(int));
    int *send_displs = (int *)malloc(size * sizeof(int));
    recv_displs[0] = 0;
    send_displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        recv_displs[i] = recv_displs[i - 1] + recv_counts[i - 1];
        send_displs[i] = send_displs[i - 1] + bucket_sizes[i - 1];
    }

    int total_recv_count = 0;
    for (int i = 0; i < size; i++)
    {
        total_recv_count += recv_counts[i];
    }

    int *recv_bucket = (int *)malloc(total_recv_count * sizeof(int));
    if (recv_bucket == NULL)
    {
        perror("Error allocating memory for receive buckets");
        for (int k = 0; k < num_buckets; k++)
        {
            free(buckets[k]);
        }
        free(buckets);
        free(bucket_sizes);
        free(local_data);
        free(send_displs);
        free(recv_displs);
        free(recv_counts);
        MPI_Finalize();
        exit(1);
    }

    int *send_data = (int *)malloc(local_N * sizeof(int));
    int send_data_index = 0;
    for (int k = 0; k < num_buckets; k++)
    {
        for (int j = 0; j < bucket_sizes[k]; j++)
        {
            send_data[send_data_index++] = buckets[k][j];
        }
    }

    MPI_Alltoallv(send_data, bucket_sizes, send_displs, MPI_INT,
                  recv_bucket, recv_counts, recv_displs, MPI_INT,
                  MPI_COMM_WORLD);


    bubble_sort(recv_bucket, total_recv_count);


    int *recv_counts_prefix_sum = (int *)malloc(size * sizeof(int));
    int *global_offsets = (int *)malloc(size * sizeof(int));

    MPI_Exscan(&total_recv_count, &recv_counts_prefix_sum[rank], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
        recv_counts_prefix_sum[rank] = 0;

    MPI_Allgather(&recv_counts_prefix_sum[rank], 1, MPI_INT, global_offsets, 1, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0)
    {
        global_offsets[0] = 0;
    }
    for (int i = 1; i < size; i++)
    {
        global_offsets[i] = global_offsets[i - 1] + recv_counts[i - 1];
    }

    MPI_File fout;
    MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fout);
    MPI_File_write_at(fout, global_offsets[rank] * sizeof(int), recv_bucket, total_recv_count, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fout);

    for (int k = 0; k < num_buckets; k++)
    {
        free(buckets[k]);
    }
    free(buckets);
    free(bucket_sizes);
    free(recv_bucket);
    free(local_data);
    free(send_displs);
    free(recv_displs);
    free(recv_counts);
    free(send_data);
    free(recv_counts_prefix_sum);
    free(global_offsets);

    MPI_Finalize();
    return 0;
}
