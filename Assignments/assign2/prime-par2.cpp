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
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;
int totalPrimes = 0;
vector<int> sieve;
int totalSieves = 0;

void worker(int start, int end, int k, int P)
{
    unique_lock<mutex> lk(mtx);
    int range = end - start + 1;
    int remain = range % P;
    int startIdx = (range / P) * k + min(k, remain) + start;
    int endIdx = startIdx + (range / P) - 1 + (k < remain ? 1 : 0);

    int primesFound = 0;
    bool *candidate = new bool[endIdx - startIdx + 1];
    for (int i = 0; i <= endIdx - startIdx; i++)
        candidate[i] = true;

    int lastProcessedSieve = -1;

    while (!ready || lastProcessedSieve + 1 < totalSieves)
    {
        cv.wait(lk, [&]
                { return ready || lastProcessedSieve + 1 < totalSieves; });

        while (lastProcessedSieve + 1 < totalSieves)
        {
            int prime = sieve[lastProcessedSieve + 1];
            int firstMultiple = max(prime * prime, ((startIdx + prime - 1) / prime) * prime);
            for (int j = firstMultiple; j <= endIdx; j += prime)
                if (j >= startIdx)
                    candidate[j - startIdx] = false;
            lastProcessedSieve++;
        }
    }
    lk.unlock();

    for (int i = 0; i <= endIdx - startIdx; i++)
    {
        if (candidate[i])
            primesFound++;
    }
    delete[] candidate;

    // printf("Worker[%d] found %d primes in [%d..%d]\n", k, primesFound, startIdx, endIdx);

    lock_guard<mutex> guard(mtx);
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

    // printf("prime-par2 (%d threads) over [%d..%d] ...\n", P, 2, N);
    auto start = chrono::steady_clock::now();

    int sqrtN = sqrt(N);
    bool candidate[sqrtN + 1];

    thread workerTh[P];
    for (int i = 0; i < P; i++)
        workerTh[i] = thread(worker, sqrtN + 1, N, i, P);

    for (int i = 2; i <= sqrtN; i++)
        candidate[i] = true;

    for (int i = 2; i <= sqrt(N); i++)
        if (candidate[i])
        {
            lock_guard<mutex> lk(mtx);
            sieve.push_back(i);
            totalSieves++;
            totalPrimes++;
            cv.notify_all();
            for (int j = i + i; j <= sqrtN; j += i)
                candidate[j] = false;
        }

    ready = true;
    cv.notify_all();

    // printf("Master found %d primes in [%d..%d]\n", totalSieves, 2, sqrtN);

    for (int i = 0; i < P; i++)
        workerTh[i].join();

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration<double, std::milli>(end - start).count();

    cout << "prime-par2 (N=" << N << ") found " << totalPrimes << " primes in " << duration << "ms\n";
}
