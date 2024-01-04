//////////////////////////////////////////////////////////////////////////////
///
/// @copyright
/// Copyright (C) 2021 Remote Diagnostic Technologies Ltd.
///
/// @brief
///    This class is responsible to measure the CPU and RAM memory
///
/// @author Qt Creator Wizard and Binay Mandal.
///
//////////////////////////////////////////////////////////////////////////////
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <ProcessMonitor.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Constructor to initialize variables.
///
/// @param none
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
ProcessMonitor::ProcessMonitor():m_system(NULL),
  m_statusMutex(NULL) {
   // std::cout<<"Created obj of ProcessMonitor"<<std::endl;
  m_pageSizeInKB = sysconf(_SC_PAGE_SIZE) / 1024;

  // For unit testing ==============
  m_sampleCount = 0;
  m_average_on_averageCPU = 0.0;
  m_sum_of_averageCPU = 0.0;

  m_average_on_CPU1 = 0.0;
  m_sum_of_CPU1 = 0.0;

  m_average_on_CPU2 = 0.0;
  m_sum_of_CPU2 = 0.0;
  //================================
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Destructor to delete variables.
///
/// @param none
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
ProcessMonitor::~ProcessMonitor() {
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Initializes variables like system module pointer, mutex etc.
///
/// @param system system module pointer
/// @param statusVarMutextPtr mutex for status variable
/// @param statusVarMutextPtr status variable
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
// void ProcessMonitor::initialise(System *system,
//                                 pthread_mutex_t * statusVarMutextPtr) {
//   if (system != nullptr) {
//     m_system = system;
//     m_statusMutex = statusVarMutextPtr;
//   }
// }

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Gets current CPU value.
///
/// @param none
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
void ProcessMonitor::getCPU() {
  std::string cpuVal = printCPU();
//   pthread_mutex_lock(m_statusMutex);
//   std:string m_cpu = cpuVal;
//   pthread_mutex_unlock(m_statusMutex);

  std::string cpuString("Idle CPU:(Avg, CPU0, CPU1): ");
  cpuString = cpuString + cpuVal;
  std::cout  << cpuString << "\t";
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Gets current RAM memory value.
///
/// @param none
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
void ProcessMonitor::getRAMMemory() {
  std::string ramVal = printRAM();

  std::string ramString("RAM: ");
  ramString = ramString + ramVal;
  std::cout << ramString << "\t" ;
  // std::cout << ram.getString() << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates CPU usuage based on two CPU snapshots.
///
/// @param none
///
/// @return string
//////////////////////////////////////////////////////////////////////////////
std::string ProcessMonitor::printCPU() {
  ProcessResourceUsage instance_1 = {};
  ProcessResourceUsage instance_2 = {};

  readCPU(&instance_1);
  sleep(1);
  readCPU(&instance_2);

  double cpu_0_total_jiffies = instance_2.cpu_0_jiffies
      - instance_1.cpu_0_jiffies;
  double cpu_1_total_jiffies = instance_2.cpu_1_jiffies
      - instance_1.cpu_1_jiffies;
  double cpu_0_total_work = instance_2.cpu_0_work
      - instance_1.cpu_0_work;
  double cpu_1_total_work = instance_2.cpu_1_work
      - instance_1.cpu_1_work;
  double cpu_0 = (cpu_0_total_work * 100)
      / cpu_0_total_jiffies;
  double cpu_1 = (cpu_1_total_work * 100)
      / cpu_1_total_jiffies;

  // Get idle cpu %
  cpu_0 = 100 - cpu_0;
  cpu_1 = 100 - cpu_1;

  double average_idle_cpu = (cpu_0 + cpu_1) / 2;

  // Unit test code: ignore 2 minutes sample, and then calculate average CPU
  // in first 2 minute
//  if (m_sampleCount != -1) {
//      m_sampleCount++;
//      if (m_sampleCount > 40) {
//          m_sum_of_averageCPU += average_idle_cpu;

//          m_sum_of_CPU1 += cpu_0;
//          m_sum_of_CPU2 += cpu_1;

//          if (m_sampleCount == 80) {
//              m_average_on_averageCPU = m_sum_of_averageCPU/40;
//              m_average_on_CPU1 = m_sum_of_CPU1/40;
//              m_average_on_CPU2 = m_sum_of_CPU2/40;
//          }
//      }
//  }

  char tmp[100];
// // Unit test code:Display 2 min average
//  snprintf(tmp, sizeof (tmp), "%0.1f, %0.1f, %0.1f, %0.1f",
//           m_average_on_averageCPU,
//           m_average_on_CPU1,
//           m_average_on_CPU2,
//           average_idle_cpu);


    snprintf(tmp, sizeof (tmp), "%0.1f, %0.1f, %0.1f", average_idle_cpu,
             cpu_0,
             cpu_1);

  return tmp;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates RAM usuage.
///
/// @param none
///
/// @return string
//////////////////////////////////////////////////////////////////////////////
std::string ProcessMonitor::printRAM() {
  int32_t ramVal = 0;
  readRAM(&ramVal);
  char tmp[100];
  snprintf(tmp, sizeof (tmp), "%d", ramVal);
  return tmp;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Reads current CPU states and calculates idle and total time.
///
/// @param pusage states for current cpu snap
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
void ProcessMonitor::readCPU(ProcessResourceUsage *pusage) {
  std::ifstream proc_stat("/proc/stat");
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(proc_stat, line)) {
    lines.push_back(line);
  }
  proc_stat.close();

  std::vector<size_t> cpu0_times, cpu1_times;
  pusage->cpu_0_jiffies = getTotalCPUTime(lines.at(1), &cpu0_times);
  pusage->cpu_1_jiffies = getTotalCPUTime(lines.at(2), &cpu1_times);

//  pusage->cpu_0_idle = cpu0_times.at(STATE_IDLE)
//      + cpu0_times.at(STATE_IOWAIT);


//  pusage->cpu_1_idle = cpu1_times.at(STATE_IDLE)
//      + cpu1_times.at(STATE_IOWAIT);

  pusage->cpu_0_work = cpu0_times.at(STATE_USER)
      + cpu0_times.at(STATE_SYSTEM)
      + cpu0_times.at(STATE_NICE);


  pusage->cpu_1_work = cpu1_times.at(STATE_USER)
          + cpu1_times.at(STATE_SYSTEM)
          + cpu1_times.at(STATE_NICE);
}
//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Extracts CPU states and store in a vector.
///
/// @param cpu a string contains all cpu times
/// @param times a vector holds all the extracted cpu times
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
int32_t ProcessMonitor::getTotalCPUTime(
    const std::string &cpu,
    std::vector<size_t> *times) {

  std::istringstream ss(cpu);
  std::string ignore;
  ss >> ignore;
  for (size_t time; ss >> time; times->push_back(time));


  int32_t total = times->at(STATE_USER)
                  + times->at(STATE_SYSTEM)
                  + times->at(STATE_NICE)
                  + times->at(STATE_IDLE)
                  + times->at(STATE_IOWAIT)
                  + times->at(STATE_IRQ)
                  + times->at(STATE_SOFTIRQ)
                  + times->at(STATE_STEAL)
                  + times->at(STATE_GUEST)
                  + times->at(STATE_GUEST_NICE);

  return total;
}
//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Reads current RAM value.
///
/// @param memory holds the final value
///
/// @return none
//////////////////////////////////////////////////////////////////////////////
void ProcessMonitor::readRAM(int32_t *memory) {
  int32_t tSize = 0, resident = 0, share = 0;
  std::ifstream buffer("/proc/self/statm");
  buffer >> tSize >> resident >> share;
  buffer.close();

  *memory = resident * m_pageSizeInKB;
}


