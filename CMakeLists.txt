cmake_minimum_required(VERSION 3.0.0)
project(Benchmark VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include(grpc_protobuf_config.cmake)

add_custom_command(
    OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${hw_proto_path}"
        --cpp_out "${hw_proto_path}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")


# set(SOURCES
#     trivial.ipc.pb.cc
#     trivial.ipc.grpc.pb.cc
#     grpc-ipc-latency.cpp
# )

set(SOURCES_CLIENT
    trivial.ipc.pb.cc
    trivial.ipc.grpc.pb.cc
    ipc-latency-client.cpp
    CpuMemoryMonitor.cpp
    ProcessMonitor.cpp
)
set(SOURCES_SERVER
    trivial.ipc.pb.cc
    trivial.ipc.grpc.pb.cc
    ipc-latency-server.cpp
)

add_executable(uds-ipc-latency uds-ipc-latency.cpp)
target_include_directories(uds-ipc-latency PRIVATE ${PROTOBUF_GRPC_INCLUDE_DIRS})
target_link_libraries(uds-ipc-latency PRIVATE rt dl)

add_executable(ipc-latency-client ${SOURCES_CLIENT})
target_include_directories(ipc-latency-client PRIVATE ${PROTOBUF_GRPC_INCLUDE_DIRS})
target_link_libraries(ipc-latency-client PRIVATE ${_REFLECTION}    ${_GRPC_GRPCPP}    ${_PROTOBUF_LIBPROTOBUF})

add_executable(ipc-latency-server ${SOURCES_SERVER})
target_include_directories(ipc-latency-server PRIVATE ${PROTOBUF_GRPC_INCLUDE_DIRS})
target_link_libraries(ipc-latency-server PRIVATE ${_REFLECTION}    ${_GRPC_GRPCPP}    ${_PROTOBUF_LIBPROTOBUF})


# Add generated files to the ipc-latency target
#target_sources(ipc-latency PRIVATE ${PROTO_SRCS} ${GRPC_SRCS})
