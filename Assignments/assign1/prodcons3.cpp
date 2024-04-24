#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sched.h>
#include "queue.h"

int BUFSIZE = 20;
int NUMITEMS = 100;
int numProd = 1;
int numCons = 1;
Queue queue(20);
std::mutex mtx;

std::condition_variable notFull;
std::condition_variable notEmpty;

void producer(int k)
{
    int remain = NUMITEMS % numProd;
    int startIdx = (NUMITEMS / numProd) * k + std::min(k, remain) + 1;
    int endIdx = startIdx + (NUMITEMS / numProd) - 1 + (k < remain ? 1 : 0);

    printf("Producer [%d] starting on core %d [%d..%d]\n", k, sched_getcpu(), startIdx, endIdx);
    for (int i = startIdx; i <= endIdx; i++)
    {
        std::unique_lock<std::mutex> lck(mtx);
        notFull.wait(lck, []{ return !queue.isFull(); });
        queue.add(i);
        int sz = queue.size();
        lck.unlock();
        notEmpty.notify_one();

        printf("P[%d] added %d (qsz: %d)\n", k, i, sz);
    }

    printf("Producer [%d] ending\n", k);
}

void consumer(int k)
{
    printf("Consumer [%d] starting on core %d\n", k, sched_getcpu());
    int removedArr[NUMITEMS] = {0};
    int counter = 0;
    int removed = 0;
    while (true)
    {
        std::unique_lock<std::mutex> lck(mtx); 
        notEmpty.wait(lck, []{ return !queue.isEmpty(); }); 
        int removed = queue.remove();
        if (removed == -1)
        {                 
            lck.unlock(); 
            break;
        }
        int sz = queue.size();
        lck.unlock();         
        notFull.notify_one(); 

        printf("C[%d] rem'd %d (qsz: %d)\n", k, removed, sz);
        removedArr[counter] = removed;
        counter++;
    }

    std::unique_lock<std::mutex> lck(mtx);
    printf("Consumer stats: [");
    int sum = 0;
    for (int i = 0; i < counter; i++)
    {
        sum += removedArr[i];
        printf("%d%s", removedArr[i], i != counter - 1 ? "," : "");
    }
    printf("] total = %d\n", sum);
    lck.unlock();
}

int main(int argc, char **argv)
{
    if (argc == 2)
        numProd = std::stoi(argv[1]) > 0 ? std::stoi(argv[1]) : 1;
    else if (argc == 3)
    {
        numProd = std::stoi(argv[1]) > 0 ? std::stoi(argv[1]) : 1;
        numCons = std::stoi(argv[2]) > 0 ? std::stoi(argv[2]) : 1;
    }

    std::thread prodThd[numProd];
    std::thread conThd[numCons];

    for (int i = 0; i < numProd; i++)
        prodThd[i] = std::thread(producer, i);

    for (int i = 0; i < numCons; i++)
        conThd[i] = std::thread(consumer, i);

    for (int i = 0; i < numProd; i++)
        prodThd[i].join();

    for (int i = 0; i < numCons; i++)
    {
        std::unique_lock<std::mutex> lck(mtx);
        queue.add(-1);
    }
    notEmpty.notify_all();

    for (int i = 0; i < numCons; i++)
        conThd[i].join();

    printf("Main: all done!\n");
    return EXIT_SUCCESS;
}