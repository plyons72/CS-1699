#ifndef CSMA_H
#define CSMA_H

class CarrierSenseMultipleAccess
{
public:
  // Set the status of the medium
  bool check_status();
  
private:
  // Total number of threads we will use throughout the execution of the program
  static const int m_numThreads;
  
  // Total number of packets each station should send
  static const int m_numPackets;
  
  // The number to be used as our unit of time... The amount of time that a station will wait before it checks the medium
  static const int m_ts;
  
  // To be used as a multiplier with ts to determine the amount of time that each station sleeps before checking the medium
  static const int m_td;
  
  // To be used as a multiplier with ts to set the amount of time a device waits, once it detects the medium is not busy
  // Should only be used if the device has something to send
  static const int m_tdifs;
  
  // Total time that a device will hold connection with the medium to send a packet.
  // Should be [.3td < tp < .6 td]
  static const int m_tp;
  
  // Time required for the medium to receive an ACK after sending a packet
  static const int m_tifs;
  
  // Equal to W, where W is a random number [ts < W < 2Nts] where N is the number of wireless stations
  // All stations should be equal... In this case we set it equal to Nts
  static int m_tcw;
  
  // Variable to hold the status of the medium through which packets are sent (available = true, unavailable = false
  static bool m_mediumStatus;
  
  // Generates a random number between 1 and n (inclusive)
  int random_number_generator(int n);
  
  // Cycles the devices through the process of sleeping and checking whether it's ok to try to send data
  void sleep_and_detect(int tid);
  
  // Simulates the sending of data from the device to the medium
  void send_data(int tid, int tot);
  
  // Set the status of the medium
  bool set_status(bool status);
  
}; 
  

#endif // CSMA_H

