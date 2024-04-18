#include <stdio.h>
#include <omp.h>

int array[8] = {1, 2, 3, 4, 5, 6, 7, 8};
int results[8];    
int thread_ids[8]; 

void rec(int a[], int n, int start_idx) {
    if (n == 1) {
        results[start_idx] = a[0];      
        thread_ids[start_idx] = omp_get_thread_num(); 
    } else {
#pragma omp task shared(results, thread_ids)
        rec(a, n/2, start_idx);

#pragma omp task shared(results, thread_ids)
        rec(a + n/2, n/2, start_idx + n/2);

#pragma omp taskwait 
    }
}

int main() {
    #pragma omp parallel
    {
#pragma omp single
        rec(array, 8, 0);
    }

    
    for (int i = 0; i < 8; i++)
        printf("%d by thread %d\n", results[i], thread_ids[i]);

    return 0;
}
