#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

/* Constant variables to be used throughout program execution */

static const int num_threads = 8;
static const int ts = 300;
static const int td = 240 * ts;
static const int tdifs = 20 * ts;
static const int tcw = num_threads * ts;
static const int tp = .5 * td;
static const int tifs = 10 * ts;
static const int num_packets = 1;

// Holds the status of the medium(available = true, unavailable = false)
static bool medium_status = true;

// Mutex for the process
std::mutex medium_lock;


void sleep_and_detect(int tid)
{
    // Generates a random number between 1 and 100 to determine the percentage chance of having something to send
    // This should be mobile device specific, and each will have a different one (ideally)
    const int probability = random_number_generator(100);

    // Total time taken for one cycle to process (device has data to send, waits to connect to medium, sends, gets ACK
    // Print to file or to Standard I/O for review. Global to allow
    int total_time = 0;
    int k;

    bool readyToSend;



    if (readyToSend)
    {
        do
        {
            // If the medium is available
            if(check_status)
            {
                // Sleep for tdifs
                std::this_thread::sleep_for(std::chrono::milliseconds(tdifs));
                total_time += tdifs;

                // Set k
                k = 1;

                // Set tcw based on k
                tcw *= k;

                // Sleep for ts
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                total_time += ts;

                // Decrement tcw by ts
                tcw -= ts;

                // Check medium again
                if (check_status && tcw > 0)
                {

                }

                set_status(tid, false);
                send_data(tid, total_time);
                set_status(tid, true);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                total_time += ts;
            }

        }while(num_packets > 0);
    }



}

// If a device has something to send, and has waited a certain amount of time, we should send the data here.
// @params are the thread(device) id and the total time thus far
void send_data(int tid, int tot)
{
    // First thread that gets to here can immediately lock the send data function.
    medium_lock.lock();

    std::this_thread::sleep_for(std::chrono::milliseconds(ts));
    total_time += ts;

    // Unlock and leave
    medium_lock.unlock();
}

// Checks the status of the medium
//@return true if the medium is unlocked and available
bool check_status()
{
    return medium_status;
}

void set_status(int tid, bool status)
{
    medium_status = status;
}

// Returns an int holding a random number between 1 and the number you pass to it (inclusive)
int random_number_generator(int n)
{
    return (rand() % n + 1);
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

    // Join the threads back together to ensure they all finished
    for (int i = 0; i < num_threads; ++i)
    {
        mobile[i].join();
    }

    return 0;
}