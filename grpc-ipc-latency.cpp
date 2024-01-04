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

#include <sys/socket.h>
#include <sys/errno.h>
#include <unistd.h>

#if __linux__
#include <sched.h>
#endif

#include <grpcpp/grpcpp.h>

#include "trivial.ipc.grpc.pb.h"

using namespace std;
using namespace trivial::ipc;
using namespace grpc;
using namespace std::chrono_literals;


#define LOOP_COUNT 100000
int loop_Count = LOOP_COUNT;

void die(const char *msg)
{
    fprintf(stderr, "%s: %s (%d)\n", msg, strerror(errno), errno);
    exit(1);
}

class RPCServiceImpl final : public RPCService::Service
{
    Status UnaryCall(ServerContext * /*context*/, const UnaryCallRequest *request,
                     UnaryCallReply *reply) override
    {
        reply->set_i(request->i() + 1);
        return Status::OK;
    }
};

int main(int argc, const char *argv[])
{
    // std::cout << "no dl static linking" << std::endl;
#if __linux__
    cpu_set_t parent_mask, child_mask;

    if (argc == 3)
    {   
        int parent, child;
        sscanf(argv[1], "%d", &parent);
        std::cout << "Parent CPU : " << parent << std::endl;
        CPU_SET(parent, &parent_mask);

        sscanf(argv[2], "%d", &child);
        std::cout << "Child CPU : " << child << std::endl;
        CPU_SET(child, &child_mask);
    }
    else if(argc == 4){
        int parent, child;
        sscanf(argv[1], "%d", &parent);
        std::cout << "Parent CPU : " << parent << std::endl;
        CPU_SET(parent, &parent_mask);

        sscanf(argv[2], "%d", &child);
        std::cout << "Child CPU : " << child << std::endl;
        CPU_SET(child, &child_mask);
        sscanf(argv[3], "%d", &loop_Count);
    }
    else{
        std::cout << "Child CPU : " << 0 << std::endl;
        std::cout << "Parent CPU : " << 0 << std::endl;
        CPU_SET(0, &parent_mask);
        CPU_SET(0, &child_mask);
    }
#endif

    std::string address{"unix:///tmp/test.socket"};

    pid_t pid = fork();
    if (pid == -1)
        die("fork() failed");

    if (pid == 0)
    { // child
#if __linux__
        if (int rc = sched_setaffinity(0, sizeof(child_mask), &child_mask); rc == -1)
            die("sched_setaffinity() failed at child");
#endif
        // std::cout << "At child Site" << std::endl;
        std::this_thread::sleep_for(100ms); // wait for setup

        auto client = RPCService::NewStub(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));

        UnaryCallRequest req{};
        req.set_i(0);

        UnaryCallReply rep{};
        // for (int i = 0; i < 1000; i++)
        // { // warmup
        //     ClientContext ctx{};
        //     if (grpc::Status status = client->UnaryCall(&ctx, req, &rep); !status.ok())
        //         die("UnaryCall failed");
        // }
        std::vector<int> latencies;
        std::cout << "Loop Cout : " << loop_Count << std::endl;
        latencies.reserve(loop_Count);

        auto testStart = chrono::high_resolution_clock::now();

        for (int i = 0; i < loop_Count; i++)
        {
            auto start = chrono::high_resolution_clock::now();
            ClientContext ctx{};
            if (grpc::Status status = client->UnaryCall(&ctx, req, &rep); !status.ok())
                die("UnaryCall failed");
            auto end = chrono::high_resolution_clock::now();

            latencies.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());
        }
        auto testEnd = chrono::high_resolution_clock::now();

        int testDurationMili = chrono::duration_cast<chrono::milliseconds>(testEnd - testStart).count();

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
    }
    else
    { // parent
#if __linux__
        if (int rc = sched_setaffinity(0, sizeof(parent_mask), &parent_mask); rc == -1)
            die("sched_setaffinity() failed at parent");
#endif

        RPCServiceImpl service;
        ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<Server> server(builder.BuildAndStart());
        server->Wait();
    }
}
