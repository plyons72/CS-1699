#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

#include "csma.h"

using namespace std;

/* Constant variables to be used throughout program execution */

static const int m_numThreads = 8;
static const int m_ts = 300;
static const int m_td = 240 * m_ts;
static const int m_tdifs = 20 * m_ts;
static const int m_tp = .5 * m_td;
static const int m_tifs = 10 * m_ts;
static const int m_numPackets = 1;


// Mutex for the process
std::mutex medium_lock;

// int random_number_generator(int n);
// void sleep_and_detect(int tid);
// void send_data(int tid, int tot);
// bool set_status(bool status);

void sleep_and_detect(int m_tid)
{
    // Generates a random number between 1 and 100 to determine the percentage chance that a device has something to send
    // Each station should have the same probability
    const int probability = random_number_generator(100);

    // Total time taken for one cycle to process (device has data to send, waits to connect to medium, sends, gets ACK
    // Print to file or to Standard I/O for review. Global to allow
    int totalTime = 0;
    int tcw = num_threads * ts;
    int k = 1;

    bool readyToSend = false;

    if (readyToSend)
    {
        do
        {
            // If the medium is available
            if(check_status())
            {
                // Sleep for tdifs
                std::this_thread::sleep_for(std::chrono::milliseconds(tdifs));
                totalTime += tdifs;

                // Set k
                k = 1;

                // Set tcw based on k
                tcw *= k;

                // Sleep for ts
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                totalTime += ts;

                // Decrement tcw by ts
                tcw -= ts;

                // Check medium again... Make sure it's available
                if (check_status() && tcw > 0)
                {

                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                totalTime += ts;
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
    totalTime += ts;

    // Unlock and leave
    medium_lock.unlock();
}

// Checks the status of the medium
//@return true if the medium is unlocked and available
bool check_status()
{
    return mediumStatus;
}

void set_status(bool status)
{
    mediumStatus = status;
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
