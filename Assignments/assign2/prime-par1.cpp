//-----------------------------------------------------------------------------
// Program code for CS 415P/515 Parallel Programming, Portland State University
//-----------------------------------------------------------------------------

// A prime-finding program (Sequential version).
//
// Usage:
//   linux> ./prime N
//
#include <iostream>
#include <cmath>
using namespace std;

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

    int sqrtN = sqrt(N);
    bool candidate[N+1];
    int sieve[sqrtN];
}
