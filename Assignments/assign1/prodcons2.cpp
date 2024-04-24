#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sched.h>
#include "queue.h"

int BUFSIZE = 20;
int NUMITEMS = 100;
int numCons = 1;
Queue queue(20);
std::mutex mtx;

std::condition_variable notFull;
std::condition_variable notEmpty;

void producer()
{
    printf("Producer starting on core %d\n", sched_getcpu());
    for (int i = 1; i <= NUMITEMS; i++)
    {
        std::unique_lock<std::mutex> lck(mtx);
        notFull.wait(lck, []{ return !queue.isFull(); });
        queue.add(i);
        int sz = queue.size();
        lck.unlock();
        notEmpty.notify_one();

        printf("Producer added %d (qsz: %d)\n", i, sz);
    }

    std::unique_lock<std::mutex> lck(mtx);
    for (int i = 0; i < numCons; i++)
    {
        notFull.wait(lck, []{ return !queue.isFull(); });
        queue.add(-1);
        notEmpty.notify_one();
    }

    printf("Producer ending\n");
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
    if (argc != 1)
        numCons = std::stoi(argv[1]) > 0 ? std::stoi(argv[1]) : 1;

    std::thread conThd[numCons];
    for (int i = 0; i < numCons; i++)
        conThd[i] = std::thread(consumer, i);

    std::thread thd1(producer);

    thd1.join();
    for (int i = 0; i < numCons; i++)
        conThd[i].join();

    printf("Main: all done!\n");
    return EXIT_SUCCESS;
}