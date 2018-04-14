#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

#define NUM_THREADS 8
#define UNIT_TIME 100
#define DATA_READY_CHECK_MULTIPLIER 240
std::mutex csma_lock;

void call_from_thread(int tid)
{
    // Generates a random number between 1 and 100 to determine the percentage chance of having something to send
    // To generate a truly random and not just pseudorandom numbers, we seed the rand function with the current Unix System time in main()
    int probability = (rand() % 100)+ 1;
    printf("\n\n\nLaunched by thread %d\n", tid);
    printf("Thread %d generated the number %d\n", tid, probability);
}

int main(int argc, char *argv[])
{
    std::thread mobile[NUM_THREADS];
    srand((int)time(0));

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

