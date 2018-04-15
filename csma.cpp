#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

/* Constant variables to be used throughout program execution */

// Total number of threads we will use throughout the execution of the program
static const int num_threads = 8;

// The number to be used as our unit of time... The amount of time that a station will wait before it checks the medium
static const int ts = 100;

// To be used as a multiplier with ts to determine the amount of time that each station sleeps before checking the medium
static const int td = 240 * ts;

// To be used as a multiplier with ts to set the amount of time a device waits, once it detects the medium is not busy
// Should only be used if the device has something to send
static const int tdifs = 20 * ts;

// Equal to W, where W is a random number [ts < W < 2Nts] where N is the number of wireless stations
// All stations should be equal... In this case we set it equal to Nts
static const int tcw = num_threads * ts;

// Total time that a device will hold connection with the medium to send a packet.
// Should be [.3td < tp < .6 td]
static const int tp = .5 * td;

// Time required for the medium to receive an ACK after sending a packet
static const int tifs = 10 * ts;

// Total number of packets each station should send
static const int num_packets = 4;

std::mutex medium_lock;

void sleep_and_detect(int tid)
{
    // Generates a random number between 1 and 100 to determine the percentage chance of having something to send
    // This should be mobile device specific, and each will have a different one (ideally)
    const int probability = (rand() % 100)+ 1;

    // Total time taken for one cycle to process (device has data to send, waits to connect to medium, sends, gets ACK
    // Print to file or to Standard I/O for review. Global to allow
    int total_time;

    printf("\n\n\nLaunched by thread %d\n", tid);
    printf("Thread %d generated the number %d\n", tid, probability);
}

// If a device has something to send, and has waited a certain amount of time, we should send the data here.
// @params are the thread(device) id and the total time thus far
void send(int tid, int tot)
{

}

int main(int argc, char *argv[])
{
    // Create the 8 mobile devices as individual threads
    std::thread mobile[num_threads];

    // Seed the rand function using current unix system time to give us truly random numbers when we call for them
    srand((int)time(0));

    // Have the threads call the sleep_and_detect function
    for (int i = 0; i < num_threads; ++i)
    {
        mobile[i] = std::thread(sleep_and_detect, i);
    }

    // Join the threads back together before we exit
    for (int i = 0; i < num_threads; ++i)
    {
        mobile[i].join();
    }

    return 0;
}

