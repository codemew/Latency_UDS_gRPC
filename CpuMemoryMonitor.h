
#include <memory>


class ProcessMonitor;

class CpuMemoryMonitor
{
private:
    /* data */

    std::unique_ptr<ProcessMonitor> m_processMonitor;
public:
    CpuMemoryMonitor(/* args */);
    ~CpuMemoryMonitor();
    void CpuMemoryMonitorThreadFunction();

};

