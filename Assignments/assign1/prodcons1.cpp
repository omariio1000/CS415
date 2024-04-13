#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sched.h>
#include "queue.h"

using namespace std;

int BUFSIZE = 20;
int NUMITEMS = 100;
Queue queue(20);
mutex mtx;
conditional_variable notFull;
conditional_variable notEmpty;

void producer() {
    cout << "Producer starting on core " << sched_getcpu() << endl;

    for (int i = 1; i <= NUMITEMS; i++) {

    }

    cout << "Producer ending" << endl;
}

void consumer() {
    cout << "Consumer starting on core " << sched_getcpu() << endl;

    for (int i = 1; i <= NUMITEMS; i++) {
        
    }

    cout << "Consumer ending" << endl;
}



int main() {

    return EXIT_SUCCESS;
}