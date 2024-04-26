//-----------------------------------------------------------------------------
// Program code for CS 415/515 Parallel Programming, Portland State University.
//-----------------------------------------------------------------------------

// The sum program (Sequential version).
//
//
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

atomic<long> sum(0);

long compute(long i) { return sqrt(i * i); }

void worker(int id, long blockSize, int N, int P)
{
  long local_sum = 0;
  long start = id * blockSize;
  long end = (id == P - 1) ? N : start + blockSize;
  for (long i = start; i < end; i++)
  {
    local_sum += compute(i);
  }
  sum += local_sum;
  cout << "Worker[" << id << "] on range [" << start << ".." << end << ")\n";
}

int main(int argc, char **argv)
{
  long N = 0;
  int P = 1; // Default number of threads

  if (argc < 2)
  {
    cout << "Usage: ./sum-par N [P]\n";
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  if (argc >= 3)
  {
    P = strtol(argv[2], NULL, 10);
  }

  std::thread threads[P];
  long blockSize = N / P;
  auto start = chrono::steady_clock::now();

  for (int i = 0; i < P; i++)
  {
    threads[i] = std::thread(worker, i, blockSize, N, P);
  }

  for (auto &t : threads)
  {
    t.join();
  }

  auto end = chrono::steady_clock::now();
  auto duration = chrono::duration<double, std::milli>(end - start).count();

  cout << "Sum of " << N << " sqrt(i*i) is  " << sum << endl;
  cout << "C++-version (" << P << " threads) elapsed time: " << duration << " ms\n";
}
