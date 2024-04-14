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

void producer() {
    printf("Producer starting on core %d\n", sched_getcpu());
    for (int i = 1; i <= NUMITEMS; i++) {
        std::unique_lock<std::mutex> lck(mtx);
        
        notFull.wait(lck, []{ return !queue.isFull(); });
        queue.add(i);
        std::cout << "Producer added " << i << "(qsz: " << queue.size() << ")" << std::endl;
        notEmpty.notify_one();
    }

    printf("Producer ending\n");
}

void consumer() {
    std::cout << "Consumer starting on core " << sched_getcpu() << std::endl;
    for (int i = 1; i <= NUMITEMS; i++) {
        std::unique_lock<std::mutex> lck(mtx);

        notEmpty.wait(lck, []{ return !queue.isEmpty(); });        
        printf("Consumer rem'd %d (qsz: %d)\n", queue.remove(), queue.size());
        notFull.notify_one();
    }
    printf("Consumer ending\n");
}



int main() {
    std::thread thd1(producer), thd2(consumer);
    thd1.join();
    thd2.join();
    printf("Main: all done!\n");
    return EXIT_SUCCESS;
}