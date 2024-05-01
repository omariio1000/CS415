//-----------------------------------------------------------------------------
// Program code for CS 415P/515 Parallel Programming, Portland State University
//-----------------------------------------------------------------------------

// A prime-finding program (Sequential version).
//
// Usage:
//   linux> ./prime N
// 0
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
using namespace std;

mutex totalPrimesMutex;
int totalPrimes = 0;

void worker(int start, int end, int k, int P, const vector<int> &sieve)
{
    int range = end - start + 1;
    int remain = range % P;
    int startIdx = (range / P) * k + std::min(k, remain) + start;
    int endIdx = startIdx + (range / P) - 1 + (k < remain ? 1 : 0);

    int primesFound = 0;

    bool candidate[endIdx - startIdx + 1];
    bool sieveArr[start];

    for (int i = 0; i < max(endIdx - startIdx + 1, start); i++)
    {
        if (i < range)
            candidate[i] = true;
        if (i < start)
            sieveArr[i] = false;
    }

    for (int prime : sieve)
        sieveArr[prime] = true;

    for (int i = 2; i <= start - 1; i++)
        if (sieveArr[i])
            for (int j = i + i; j <= endIdx; j += i)
                if (j >= startIdx)
                    candidate[j - startIdx] = false;

    for (int i = 0; i < endIdx - startIdx + 1; i++)
        if (candidate[i])
            primesFound++;

    printf("Worker[%d] found %d primes in [%d..%d]\n", k, primesFound, startIdx, endIdx);

    lock_guard<mutex> guard(totalPrimesMutex);
    totalPrimes += primesFound;
}

int main(int argc, char **argv)
{
    int N;
    int P = 1;
    if (argc < 2)
    {
        cout << "Usage: ./prime-parX N [P]\n";
        exit(0);
    }
    if ((N = atoi(argv[1])) < 2)
    {
        cout << "N must be greater than 1\n";
        exit(0);
    }
    if (argc > 2)
    {
        P = atoi(argv[2]);
        if (P < 1)
        {
            cout << "P must be at least 1\n";
            exit(0);
        }
    }

    printf("prime-par1 (%d threads) over [%d..%d] ...\n", P, 2, N);
    auto start = chrono::steady_clock::now();
    int sqrtN = sqrt(N);
    bool candidate[sqrtN + 1];
    vector<int> sieve;

    for (int i = 2; i <= sqrtN; i++)
        candidate[i] = true;

    for (int i = 2; i <= sqrt(N); i++)
        if (candidate[i])
        {
            sieve.push_back(i);
            for (int j = i + i; j <= sqrtN; j += i)
                candidate[j] = false;
        }

    for (int i = 2; i <= sqrtN; i++)
        if (candidate[i])
            totalPrimes++;

    printf("Master found %d primes in [%d..%d]\n", totalPrimes, 2, sqrtN);

    thread workerTh[P];
    for (int i = 0; i < P; i++)
        workerTh[i] = thread(worker, sqrtN + 1, N, i, P, cref(sieve));

    for (int i = 0; i < P; i++)
        workerTh[i].join();

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration<double, std::milli>(end - start).count();

    printf("prime-par1 (N=%d, P=%d) found %d primes in %f ms\n", N, P, totalPrimes, duration);
}
