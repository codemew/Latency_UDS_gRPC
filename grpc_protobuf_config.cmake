# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
if(CMAKE_CROSSCOMPILING)
find_package(Protobuf REQUIRED)
else()
find_package(Protobuf REQUIRED)       #Do not use cmake CONFIG, if using old versions of protobuf version
endif()
message(STATUS "Using protobuf version ${protobuf_VERSION}")

# Protobuf-compiler
if(CMAKE_CROSSCOMPILING)
find_program(_PROTOBUF_PROTOC protoc)
else()
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()


# # Proto file
get_filename_component(hw_proto "${CMAKE_SOURCE_DIR}/trivial.ipc.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

message(STATUS "hw_proto      : ${hw_proto}")
message(STATUS "hw_proto_path : ${hw_proto_path}")

# # Generated sources
message(STATUS "COMMON_GRPC_PROTO_PATH : ${hw_proto_path}")
#set(GRPC_PROTOFILES_DIR "${COMMON_SOURCE_PATH}/GRPC/")


set(hw_proto_srcs "${hw_proto_path}/trivial.ipc.pb.cc")
set(hw_proto_hdrs "${hw_proto_path}/trivial.ipc.pb.h")
set(hw_grpc_srcs "${hw_proto_path}/trivial.ipc.grpc.pb.cc")
set(hw_grpc_hdrs "${hw_proto_path}/trivial.ipc.grpc.pb.h")


# Include generated *.pb.h files
include_directories("${hw_proto_path}")