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

//#include <sys/socket.h>
#include <sys/errno.h>
#include <unistd.h>

#if __linux__
#include <sched.h>
#endif

#define GRPC_COMMS

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
std::vector<int32_t> latencies;
int32_t recvdMessageCount = 0;
uint64_t testStart = 0;
// int64_t msgRecvTime = 0;
// int64_t msgSendTime = 0;
uint64_t msgLatency  = 0;

#define FORMATTED_TIME(time)                ((time >= 1000) ? (double) time/1000 : time) 
#define FORMATTED_TIME_UNIT(time)           ((time >= 1000) ? " ms" : " us" )                      //input should be in microseconds


static inline void benchmarkData( ){

        uint64_t testEnd = COMMONUTILS::getTickCountInMicroSec();

        uint64_t testDurationMili = (testEnd - testStart)/1000;             //ms

        auto minLatency = std::min_element(latencies.begin(), latencies.end());     //us
        auto maxLatency = std::max_element(latencies.begin(), latencies.end());     //us

        // Calculate average using std::accumulate and division
        double averageLatency = static_cast<double>(std::accumulate(latencies.begin(), latencies.end(), 0)) / latencies.size();     //us

        // Output results
        std::cout << "Minimum Latency: " << FORMATTED_TIME(*minLatency) << FORMATTED_TIME_UNIT(*minLatency) << std::endl;           //
        std::cout << "Maximum Latency: " << FORMATTED_TIME(*maxLatency) << FORMATTED_TIME_UNIT(*maxLatency) << std::endl;
        std::cout << "Average Latency: " << FORMATTED_TIME(averageLatency) << FORMATTED_TIME_UNIT(averageLatency) << std::endl;

        // printf("%i us\n", l);
        std::cout << "Test Duration " << (testDurationMili)/1000 << " Sec , " << (testDurationMili)%1000 << " ms " <<std::endl;
        std::cout << "Test Ended!!! \n" << std::endl;
        latencies.clear();
        testStart = 0;
        recvdMessageCount = 0;
}


void die(const char *msg)
{
    fprintf(stderr, "%s: %s (%d)\n", msg, strerror(errno), errno);
    exit(1);
}
#ifdef GRPC_COMMS
class RPCServiceImpl final : public RPCService::Service
{
    Status UnaryCall(ServerContext * /*context*/, const UnaryCallRequest *request,
                     UnaryCallReply *reply) override
    {
        //reply->set_i(request->i() + 1);
        msgLatency  = COMMONUTILS::getTickCountInMicroSec() - request->timestamp();

        if( recvdMessageCount == 0 ){
             testStart = COMMONUTILS::getTickCountInMicroSec();     //total Test duration
        }
        latencies.emplace_back(msgLatency);

        //todo Implementation
        recvdMessageCount++;
        if( recvdMessageCount == loop_Count){
            std::cout<<"Finished receiving... Message Received Count: " <<  recvdMessageCount  << std::endl;
            benchmarkData();
        }
        return Status::OK;
    }
};
#endif


int main(int argc, const char *argv[])
{
    // std::cout << "no dl static linking" << std::endl;
    latencies.reserve(loop_Count);
#if __linux__
    cpu_set_t cpu_mask;

    if (argc == 3)
    {   
        int cpu;

        sscanf(argv[1], "%d", &cpu);
    //    std::cout << "Server CPU Core : " << cpu << std::endl;
        CPU_SET(cpu, &cpu_mask);
        sscanf(argv[2], "%d", &loop_Count);

    }  else  {
    //    std::cout << "Server CPU Core : " << 0 << std::endl;
        CPU_SET(0, &cpu_mask);
    }
#endif




#if __linux__
/*         if (int rc = sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask); rc == -1)
            die("sched_setaffinity() failed at Server"); */
#endif

#ifdef GRPC_COMMS
        std::string address{"unix:///tmp/test.socket"};
        RPCServiceImpl service;
        ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        server->Wait();
#endif

#ifndef GRPC_COMMS
//No sleep , Main thread [no secondary thread]
    int32_t recvdMessageCount = 0;
    int64_t testStart = 0;
    std::vector<int> latencies;

    std::cout<<"Starting  receiving..."  << std::endl;
    //std::cout << "Loop Cout : " << loop_Count << std::endl;
    latencies.reserve(loop_Count);
    while(recvdMessageCount < loop_Count)
    {

        //recvd Data            // { must be a blocking call 
        int64_t msgRecvTime = COMMONUTILS::getTickCountInMicroSec();
        int64_t msgSendTime; // = payload.timestamp; 
        int64_t msgLatency = msgRecvTime - msgSendTime;

        if( recvdMessageCount == 0 ){
             testStart = msgRecvTime;     //total Test duration
        }

        latencies.emplace_back(msgLatency);

        //todo Implementation
        

        recvdMessageCount++;
    }

        std::cout<<"Finished receiving... Message Received Count: " <<  recvdMessageCount  << std::endl;

        int64_t testEnd = COMMONUTILS::getTickCountInMicroSec();

        int64_t testDurationMili = (testEnd - testStart)/1000;

        auto minLatency = std::min_element(latencies.begin(), latencies.end());
        auto maxLatency = std::max_element(latencies.begin(), latencies.end());

        // Calculate average using std::accumulate and division
        double averageLatency = static_cast<double>(std::accumulate(latencies.begin(), latencies.end(), 0)) / latencies.size();

        // Output results
        std::cout << "Minimum Latency: " << ((*minLatency >= 1000) ? (double) *minLatency/1000 : *minLatency) << ((*minLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Maximum Latency: " << ((*maxLatency >= 1000) ? (double) *maxLatency/1000 : *maxLatency) << ((*maxLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Average Latency: " << ((averageLatency >= 1000) ? (double) averageLatency/1000 : averageLatency) << ((averageLatency >= 1000) ? " ms" : " us" ) << std::endl;

        // printf("%i us\n", l);
        std::cout << "Test Duration " << (testDurationMili)/1000 << " Sec , " << (testDurationMili)%1000 << " ms " <<std::endl;
        std::cout << "Exit now!!!" << std::endl;

#endif
return 0;
}
