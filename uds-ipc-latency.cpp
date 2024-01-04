// uds-ipc-latency.cpp - Measure latency of unary IPC calls over a Unix domain socket using blocking I/O.
//
// Compile: g++ -std=c++17 -O2 -march=native -mtune=native -o uds-ipc-latency uds-ipc-latency.cpp
// Run on 1st and 2nd core: ./uds-ipc-latency 0x1 0x2 > uds-ipc-latencies-nsec.txt
#include <algorithm>
#include <numeric>
#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <sys/errno.h>
#include <unistd.h>

#if __linux__
#include <sched.h>
#endif

#define LOOP_COUNT 10000
int loop_Count = LOOP_COUNT;

using namespace std;

struct DummyMsg {
    int64_t i;
};

void die(const char *msg) {
    fprintf(stderr, "%s: %s (%d)\n", msg, strerror(errno), errno);
    exit(1);
}

void call(int socket, DummyMsg &msg) {
    if (int rc = write(socket, &msg, sizeof(msg)); rc != sizeof(msg))
        die("write() failed");

    if (int rc = read(socket, &msg, sizeof(msg)); rc != sizeof(msg))
        die("read() failed");
}

int main(int argc, const char *argv[])
{
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
    } else if(argc == 4) {
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

    int sockets[2];
    if (int rc = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets); rc == -1)
        die("socketpair() failed");

    pid_t pid = fork();
    if (pid == -1)
        die("fork() failed");

    if (pid == 0) {  // child
        close(sockets[0]); // close parent's socket
        #if __linux__
        if (int rc = sched_setaffinity(0, sizeof(child_mask), &child_mask); rc == -1)
            die("sched_setaffinity() failed");
        #endif

        DummyMsg msg{.i = 0};
        for (int i = 0; i < 1000; i++)
            call(sockets[1], msg);  // warmup
        std::cout << "Loop Cout : " << loop_Count << std::endl;
        std::vector<int> latencies;
        latencies.reserve(loop_Count);

        auto testStart = chrono::high_resolution_clock::now();

        for (int i = 0; i < loop_Count; i++) {
            auto start = chrono::high_resolution_clock::now();
            call(sockets[1], msg);
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
        std::cout << "Minimum Latency: " << ((*minLatency >= 1000)? (double) *minLatency/1000 : *minLatency) << ((*minLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Maximum Latency: " << ((*maxLatency >= 1000)? (double) *maxLatency/1000 : *maxLatency) << ((*maxLatency >= 1000) ? " ms" : " us" ) << std::endl;
        std::cout << "Average Latency: " << ((averageLatency >= 1000)? (double) averageLatency/1000 : averageLatency) << ((averageLatency >= 1000) ? " ms" : " us" ) << std::endl;

        // printf("%i us\n", l);
        std::cout << "Test Duration " << (testDurationMili)/1000 << " Sec , " << (testDurationMili)%1000 << " ms " <<std::endl;
        std::cout << "Exit now!!!" << std::endl;

        close(sockets[1]);
    } else {  // parent
        close(sockets[1]);  // close child's socket

        #if __linux__
        if (int rc = sched_setaffinity(0, sizeof(parent_mask), &parent_mask); rc == -1)
            die("sched_setaffinity() failed");
        #endif

        DummyMsg msg;
        while (1) {
            if (int rc = read(sockets[0], &msg, sizeof(msg)); rc != sizeof(msg))
                die("read() failed");

            msg.i++;

            if (int rc = write(sockets[0], &msg, sizeof(msg)); rc != sizeof(msg))
                die("write() failed");
        }

        close(sockets[0]);
    }
}

