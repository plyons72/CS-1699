#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>.

using namespace std;

#define NUM_THREADS 8

void call_from_thread(int tid)
{
    printf("Launched by thread %d\n", tid);
}

int main(int argc, char *argv[])
{
    std::thread mobile[NUM_THREADS];

    //Launch a group of threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        mobile[i] = std::thread(call_from_thread, i);
    }

     printf("Launched from the main\n");

    //Join the threads with the main thread
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        mobile[i].join();
    }

    return 0;
}

