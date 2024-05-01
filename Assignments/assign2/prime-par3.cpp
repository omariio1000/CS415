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
bool done = false;
int totalPrimes = 0;
vector<int> sieve;
vector<bool> candidate;
int currentSieveIndex = 0;

void worker(int k, int N)
{
    int primesWorked = 0;
    while (true)
    {
        int prime;
        {
            unique_lock<mutex> lk(mtx);

            cv.wait(lk, []
                    { return currentSieveIndex < sieve.size() || done; });

            if (done && currentSieveIndex >= sieve.size())
                break;

            prime = sieve[currentSieveIndex];
            currentSieveIndex++;
        }
        primesWorked++;

        for (int i = prime * prime; i <= N; i += prime)
        {
            lock_guard<mutex> lk(mtx);
            candidate[i] = false;
        }
    }

    // printf("Worker[%d] worked on %d primes\n", k, primesWorked);
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

    // printf("prime-par3 (%d threads) over [%d..%d] ...\n", P, 2, N);
    auto start = chrono::steady_clock::now();

    int sqrtN = sqrt(N);
    candidate.resize(N + 1, true);
    int totalSieves = 0;

    thread workerTh[P];
    for (int i = 0; i < P; i++)
        workerTh[i] = thread(worker, i, N);

    for (int i = 2; i <= sqrt(N); i++)
        if (candidate[i])
        {
            lock_guard<mutex> lk(mtx);
            sieve.push_back(i);
            totalSieves++;
            cv.notify_all();
            for (int j = i + i; j <= sqrtN; j += i)
                candidate[j] = false;
        }

    {
        lock_guard<mutex> lk(mtx);
        done = true;
        cv.notify_all();
    }

    // printf("Master found %d primes in [%d..%d]\n", totalSieves, 2, sqrtN);

    for (int i = 0; i < P; i++)
        workerTh[i].join();

    for (int i = 2; i <= N; i++)
        if (candidate[i])
            totalPrimes++;

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration<double, std::milli>(end - start).count();

    printf("prime-par3 (N=%d, P=%d) found %d primes in %f ms\n", N, P, totalPrimes, duration);
}
