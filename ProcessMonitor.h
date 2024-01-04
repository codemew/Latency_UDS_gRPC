//////////////////////////////////////////////////////////////////////////////
///
/// @copyright
/// Copyright (C) 2021 Remote Diagnostic Technologies Ltd.
///
/// @brief
///    This is the header file for the ProcessMonitor.
///    Basically variables declaration and functions declaration has been
///    done in this file.
///
/// @authors Qt Creator Wizard and Binay Mandal.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H
#pragma once
#include <string>
#include <vector>

class System;

typedef struct {
    int32_t cpu_0_jiffies;
    int32_t cpu_1_jiffies;
    int32_t cpu_0_work;
    int32_t cpu_1_work;
} ProcessResourceUsage;

/////////////////////////////////////////////////////////////////////////////
/// @brief
/// This is ProcessMonitor Class.
///
//////////////////////////////////////////////////////////////////////////////
class ProcessMonitor {
 public:
    ProcessMonitor();
    ~ProcessMonitor();
    void initialise(System *system,
                   pthread_mutex_t * statusVarMutextPtr);
    void getCPU();
    void getRAMMemory();

    enum CpuState {
      STATE_USER = 0,
      STATE_SYSTEM,
      STATE_NICE,
      STATE_IDLE,
      STATE_IOWAIT,
      STATE_IRQ,
      STATE_SOFTIRQ,
      STATE_STEAL,
      STATE_GUEST,
      STATE_GUEST_NICE
    };

 private:
    // System module pointer
    System* m_system;
    pthread_mutex_t * m_statusMutex;

    void readCPU(ProcessResourceUsage* pusage);
    void readRAM(int32_t* memory);
    void openCPU();
    void openRAM();
    std::string printCPU();
    std::string printRAM();
    int32_t getTotalCPUTime(
        const std::string &cpu,
        std::vector<size_t> *times);

    // For unit testing
    int32_t m_pageSizeInKB;
    int32_t m_sampleCount;
    double m_average_on_averageCPU;
    double m_sum_of_averageCPU;

    double m_average_on_CPU1;
    double m_sum_of_CPU1;

    double m_average_on_CPU2;
    double m_sum_of_CPU2;
};

#endif  //  PROCESSMONITOR_H
