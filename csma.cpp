#include <iostream>
#include <thread>

using namespace std;

#define NUM_THREADS = 8

void call_from_thread(void *)
{
    printf("Heres a hello message from thread %d")
}

int main(int argc, char *argv[])
{
    std::thread mobile[NUM_THREADS];

    //Launch a group of threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        mobile[i] = std::thread(call_from_thread);
    }

     std::cout << "Launched from the main\n";

    //Join the threads with the main thread
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        t[i].join();
    }

    return 0;
}

