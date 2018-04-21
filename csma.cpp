#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

// Constant variables to be used throughout program execution
static const int numThreads = 8;
static const int ts = 5;
static const int td = 240 * ts;
static const int tdifs = 20 * ts;
static const int tp = .5 * td;
static const int tifs = 10 * ts;

// Holds the status of the medium
// True = Available
// False = Unavailable
static bool mediumStatus = true;

// Mutex for the process to lock down access to the medium
std::mutex medium_lock;

// Generates a random number between 1 and n (inclusive)
int random_number_generator(int n)
{
    return (rand() % n + 1);
}

// Prints finalized data to the screen
void print_results(int deviceID, int totalTime)
{
    printf("\nDevice %d took %d ms to send 1 packet\n", deviceID, totalTime);
}

// Checks the status of the medium
//@return true if the medium is unlocked and available
bool check_status()
{
    return mediumStatus;
}

// Simulates the sending of data from the device to the medium
void set_status(bool status)
{

// Just a way to track the statuses
//    char * strStatus;
//    if (status == 1)
//    {
//        strStatus = "True";
//    }
//    else
//    {
//        strStatus = "False";
//    }
//    printf("device %d set the medium status to %s", deviceID, strStatus);

    mediumStatus = status;

}

// If a device has something to send, and has waited a certain amount of time, we should send the data here.
// @params are the thread(device) id and the total time thus far
void send_data(int deviceID, int& totalTime)
{
    // Lock and set medium to busy
    medium_lock.lock();
    set_status(false);

    // Tracks when a device begins sending data
    // printf("\n\nDevice %d is now sending data. %d milliseconds have elapsed so far\n", deviceID, totalTime);

    // Simulate time to send packet
    std::this_thread::sleep_for(std::chrono::milliseconds(tp));
    totalTime += tp;

    // Simulate time to receive ACK
    std::this_thread::sleep_for(std::chrono::milliseconds(tifs));
    totalTime += tifs;

    // Tracks when the device finishes data transfer
    //printf("Device %d is done sending data. %d milliseconds have elapsed so far\n", deviceID, totalTime);

    // Unlock and leave
    set_status(true);
    medium_lock.unlock();
}

// Cycles the devices through the process of sleeping and checking whether it's ok to try to send data
void sleep_and_detect(int deviceID, int probabilityToSend)
{
    // Total time taken for one cycle to process (device has data to send, waits to connect to medium, sends, gets ACK
    // Print to file or to Standard I/O for review. Global to allow
    int totalTime = 0;

    // Set k to 1 for the first loop to calculate tcw correctly
    int tcw = numThreads * ts;
    int k = 1;
    int numPackets = 1;

    bool incrementFirstTdifs= true;
    bool readyToSend;

    // Generate a probability for this device to have data to send
    // If so, break the loop and begin querying the medium
    // Else, sleep for td and go again
    // Phases 1 and 2
    do {

        int chanceToSend = random_number_generator(100);
        if (chanceToSend <= probabilityToSend)
        {
            readyToSend = true;

            // Simple print statement to track when a device begins to send data
            // printf("\n\nDevice %d is ready to send. Chance to send is %d and original probability is %d", deviceID, chanceToSend, probabilityToSend);

        }
        else
        {
            readyToSend = false;
            // printf("\n\nDevice %d is NOT ready to send. Chance to send is %d and original probability is %d", deviceID, chanceToSend, probabilityToSend);
        }

        if(!readyToSend)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(td));
        }

    }while(!readyToSend);

    // Query the medium and attempt to send data
    // Phases 3 4
    if (readyToSend)
    {
        do
        {
            // If the medium is available
            if(check_status())
            {
                if (incrementFirstTdifs) {
                    // Sleep for tdifs immediately
                    std::this_thread::sleep_for(std::chrono::milliseconds(tdifs));
                    totalTime += tdifs;
                }

                incrementFirstTdifs = false;

                // Set tcw based on k
                tcw *= k;

                // Sleep for ts
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                totalTime += ts;

                // Decrement tcw by ts
                tcw -= ts;

                // Check medium again... Make sure it's available
                // Step 16, No
                if (check_status() && tcw <= 0)
                {
                    send_data(deviceID, totalTime);
                    numPackets--;
                }

                // Step 16 if yes
                else if (check_status() && tcw > 0)
                {
                    // Sleep for ts
                    std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                    totalTime += ts;
                }

                // Medium is busy (Step 9 - no)
                else
                {
                    do
                    {
                        // Sleep for ts
                        std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                        totalTime += ts;
                    }while(!check_status());

                    // Sleep for tdifs
                    std::this_thread::sleep_for(std::chrono::milliseconds(tdifs));
                    totalTime += tdifs;

                    if (tcw <= 0)
                    {
                        k *= 2;
                        if (k > 16){ k = 16; }
                    }

                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(ts));
                totalTime += ts;
            }

        }while(numPackets > 0);
    }

    print_results(deviceID, totalTime);
}

int main(int argc, char *argv[])
{
  // Create the 8 mobile devices as individual threads
  std::thread mobile[numThreads];

  // Seed the rand function using current unix system time to give us truly random numbers when we call for them
  srand((int)time(0));

  // Generates a random number between 1 and 100 to determine the percentage chance that a device has something to send
  // Each station should have the same probability
  const int probabilityToSend = random_number_generator(100);

  // Have the threads call the sleep_and_detect function
  for (int i = 0; i < numThreads; ++i)
  {
    mobile[i] = std::thread(sleep_and_detect, i, probabilityToSend);
  }

  // Join the threads back together before we exit
  for (int i = 0; i < numThreads; ++i)
  {
    mobile[i].join();
  }

  return 0;
}