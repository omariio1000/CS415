#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define DATABITS 13

// Check if x is a power of 2
#define IsPowerOf2(x) (!((x) & ((x) - 1)))

// Bucket index for integer x of b bits (B is #buckets)
#define BktIdx(x, b, B) ((x) >> ((b) - (int)log2(B)))

// Print array
void print_array(int *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%4d ", a[i]);
    }
    printf("\n");
}

// Initialize array with random 13-bit int values
void init_array(int *a, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 8192;
    }
}

// Verify that array is sorted
void verify_array(int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) {
            printf("FAILED: a[%d]=%d, a[%d]=%d\n", i, a[i], i + 1, a[i + 1]);
            return;
        }
    }
    printf("%d element array is sorted.\n", n);
}

// Bubble sort
void bubble_sort(int *a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (a[i] > a[j]) {
                int tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }
}

// Bucket sort
void bucket_sort(int *a, int n, int num_buckets) {
    int bucket[num_buckets][2 * n / num_buckets];
    int bcnt[num_buckets];

    for (int k = 0; k < num_buckets; k++) {
        bcnt[k] = 0;
    }

    for (int i = 0; i < n; i++) {
        int k = BktIdx(a[i], DATABITS, num_buckets);
        bucket[k][bcnt[k]++] = a[i];
    }

    for (int k = 0; k < num_buckets; k++) {
        bubble_sort(bucket[k], bcnt[k]);
    }

    int i = 0;
    for (int k = 0; k < num_buckets; k++) {
        for (int j = 0; j < bcnt[k]; j++) {
            a[i++] = bucket[k][j];
        }
    }
}

// Main routine
int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: ./bsort-file B <infile> <outfile>\n");
        exit(0);
    }

    int B = atoi(argv[1]);
    if (!IsPowerOf2(B)) {
        printf("B (#buckets) must be a power of 2\n");
        exit(0);
    }

    FILE *fin = fopen(argv[2], "rb");
    if (fin == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    fseek(fin, 0L, SEEK_END);
    int size = ftell(fin);
    rewind(fin);

    int N = size / sizeof(int);
    int *a = (int *)malloc(N * sizeof(int));
    if (a == NULL) {
        perror("Error allocating memory");
        exit(1);
    }

    fread(a, sizeof(int), N, fin);
    fclose(fin);

    bucket_sort(a, N, B);

    FILE *fout = fopen(argv[3], "wb");
    if (fout == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    fwrite(a, sizeof(int), N, fout);
    fclose(fout);

    verify_array(a, N);
    free(a);

    return 0;
}
