#include "CpuMemoryMonitor.h"
#include "ProcessMonitor.h"
#include <unistd.h>
#include <iostream>
#include "Utility.h"


CpuMemoryMonitor::CpuMemoryMonitor()
{
    m_processMonitor = std::make_unique<ProcessMonitor>();
    COMMONUTILS::StartStandardThreadStartFunctionAsthread(this, &CpuMemoryMonitor::CpuMemoryMonitorThreadFunction ,"CPU Memory Monitoring Thread");
}

CpuMemoryMonitor::~CpuMemoryMonitor()
{
}

void CpuMemoryMonitor::CpuMemoryMonitorThreadFunction()
{
    while(true){
        m_processMonitor->getCPU();
        m_processMonitor->getRAMMemory();
        std::cout << std::endl;
        sleep(1);       // 1s
    }
}



