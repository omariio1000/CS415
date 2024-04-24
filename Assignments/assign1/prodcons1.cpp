#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sched.h>
#include "queue.h"

int BUFSIZE = 20;
int NUMITEMS = 100;
Queue queue(20);
std::mutex mtx;
std::condition_variable notFull;
std::condition_variable notEmpty;

// minimize things in critical section (like print could be cause)
// over synchronization

void producer()
{
    printf("Producer starting on core %d\n", sched_getcpu());
    for (int i = 1; i <= NUMITEMS; i++)
    {
        std::unique_lock<std::mutex> lck(mtx);

        notFull.wait(lck, []
                     { return !queue.isFull(); });
        queue.add(i);
        int sz = queue.size();
        notEmpty.notify_all();
        mtx.unlock();

        printf("Producer added %d (qsz: %d)\n", i, sz);
    }

    printf("Producer ending\n");
}

void consumer()
{
    printf("Consumer starting on core %d\n", sched_getcpu());
    for (int i = 1; i <= NUMITEMS; i++)
    {
        std::unique_lock<std::mutex> lck(mtx);

        notEmpty.wait(lck, []
                      { return !queue.isEmpty(); });
        int rem = queue.remove();
        int sz = queue.size();
        notFull.notify_all();
        mtx.unlock();

        printf("Consumer rem'd %d (qsz: %d)\n", rem, sz);
    }
    printf("Consumer ending\n");
}

int main()
{
    std::thread thd1(producer), thd2(consumer);
    thd1.join();
    thd2.join();
    printf("Main: all done!\n");
    return EXIT_SUCCESS;
}