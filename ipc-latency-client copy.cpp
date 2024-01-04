// grpc-ipc-latency.cpp - Measure latency of unary IPC calls over a Unix domain socket using gRPC.
//
// Compile: g++ -std=c++17 -O2 -march=native -mtune=native -o grpc-ipc-latency grpc-ipc-latency.cpp
// Run on 1st and 2nd core: ./grpc-ipc-latency 0x1 0x2 > grpc-ipc-latencies-nsec.txt
#include <algorithm>
#include <numeric>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>
#include "Utility.h"
#include <CpuMemoryMonitor.h>

//#include <sys/socket.h>
#include <sys/errno.h>
#include <unistd.h>

#if __linux__
#include <sched.h>
#endif

#define GRPC_COMMS
//#define RTT_STATS

#ifdef GRPC_COMMS
    #include <grpcpp/grpcpp.h>
    #include "trivial.ipc.grpc.pb.h"
    using namespace grpc;
    using namespace trivial::ipc;
#endif


using namespace std;
using namespace std::chrono_literals;


#define LOOP_COUNT 10000
int loop_Count = LOOP_COUNT;

void die(const char *msg)
{
    fprintf(stderr, "%s: %s (%d)\n", msg, strerror(errno), errno);
    exit(1);
}

struct Message{
    int64_t payload[127];
    int64_t sendTimeStamp;

    void setPayload(int data) {
         for(int i = 0; i < 127; i++)
         {
            payload[i] = data;
         }
    }
};

int main(int argc, const char *argv[])
{
    CpuMemoryMonitor CpuMemoryMonitorObj;
    // std::cout << "no dl static linking" << std::endl;
    Message msg;

#if __linux__
    cpu_set_t cpu_mask;

    if (argc == 3)
    {   
        int cpu;

        sscanf(argv[1], "%d", &cpu);
    //    std::cout << "Client CPU Core : " << cpu << std::endl;
        CPU_SET(cpu, &cpu_mask);
        sscanf(argv[2], "%d", &loop_Count);
    }
    else{
      //  std::cout << "Client CPU Core : " << 0 << std::endl;
        CPU_SET(0, &cpu_mask);
    }
#endif




#if __linux__
       /*  if (int rc = sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask); rc == -1)
            die("sched_setaffinity() failed at Client"); */
#endif
        // std::cout << "At child Site" << std::endl;
        std::this_thread::sleep_for(100ms); // wait for setup



#ifdef GRPC_COMMS
        std::string address{"unix:///tmp/test.socket"};
        // Create a default SSL ChannelCredentials object.
        auto channel_creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
        auto client = RPCService::NewStub(grpc::CreateChannel(address, /* channel_creds */ grpc::InsecureChannelCredentials()));
        UnaryCallReply rep{};
        //UnaryCallRequest req{};
#endif
    
#ifdef RTT_STATS
        std::vector<int> latencies;
        std::cout << "Loop Cout : " << loop_Count << std::endl;
        latencies.reserve(loop_Count);

        auto testStart = COMMONUTILS::getTickCountInMicroSec();
#endif
        std::cout<<"Starting transferring... Message Count: " <<  loop_Count << std::endl;
        int i = 0;
        for (; i < loop_Count; i++)
        {
            msg.setPayload(i);
#ifdef RTT_STATS
            auto start = COMMONUTILS::getTickCountInMicroSec();
#endif

#ifdef GRPC_COMMS
            ClientContext ctx{};
            UnaryCallRequest req{};

            for(int i = 0; i < 127; i++) {
                req.add_i(msg.payload[i]);
            }

            msg.sendTimeStamp = COMMONUTILS::getTickCountInMicroSec();
            req.set_timestamp(msg.sendTimeStamp);
            if (grpc::Status status = client->UnaryCall(&ctx, req, &rep); !status.ok())
                die("UnaryCall failed");    
#endif 


#ifdef RTT_STATS
            auto end = COMMONUTILS::getTickCountInMicroSec();
            latencies.emplace_back(end - start);
#endif
            //req.Clear();
        }
        std::cout<<"Finished transferring... Sent Message Count: " <<  i  << std::endl;
#ifdef RTT_STATS
        auto testEnd = COMMONUTILS::getTickCountInMicroSec();

        int testDurationMicro = (testEnd - testStart);

        auto minLatency = std::min_element(latencies.begin(), latencies.end());
        auto maxLatency = std::max_element(latencies.begin(), latencies.end());

        // Calculate average using std::accumulate and division
        double averageLatency = static_cast<double>(std::accumulate(latencies.begin(), latencies.end(), 0)) / latencies.size();

        // Output results
        std::cout << "Minimum RTT Latency: " << ((*minLatency >= 1000) ? (double) *minLatency/1000 : *minLatency) << ((*minLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Maximum RTT Latency: " << ((*maxLatency >= 1000) ? (double) *maxLatency/1000 : *maxLatency) << ((*maxLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Average RTT Latency: " << ((averageLatency >= 1000) ? (double) averageLatency/1000 : averageLatency) << ((averageLatency >= 1000) ? " ms" : " us" ) << std::endl;

        // printf("%i us\n", l);
        std::cout << "Test Duration " << (testDurationMicro)/1'000'000 << " Sec , " << (testDurationMicro)%1000 << " ms " <<std::endl;
        std::cout << "Test Ended!!!\n" << std::endl;

#endif
    
    
}
