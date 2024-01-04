// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: trivial.ipc.proto

#include "trivial.ipc.pb.h"
#include "trivial.ipc.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace trivial {
namespace ipc {

static const char* RPCService_method_names[] = {
  "/trivial.ipc.RPCService/UnaryCall",
};

std::unique_ptr< RPCService::Stub> RPCService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RPCService::Stub> stub(new RPCService::Stub(channel));
  return stub;
}

RPCService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_UnaryCall_(RPCService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RPCService::Stub::UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::trivial::ipc::UnaryCallReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_UnaryCall_, context, request, response);
}

void RPCService::Stub::experimental_async::UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_UnaryCall_, context, request, response, std::move(f));
}

void RPCService::Stub::experimental_async::UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_UnaryCall_, context, request, response, std::move(f));
}

void RPCService::Stub::experimental_async::UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_UnaryCall_, context, request, response, reactor);
}

void RPCService::Stub::experimental_async::UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_UnaryCall_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>* RPCService::Stub::AsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::trivial::ipc::UnaryCallReply>::Create(channel_.get(), cq, rpcmethod_UnaryCall_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>* RPCService::Stub::PrepareAsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::trivial::ipc::UnaryCallReply>::Create(channel_.get(), cq, rpcmethod_UnaryCall_, context, request, false);
}

RPCService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RPCService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RPCService::Service, ::trivial::ipc::UnaryCallRequest, ::trivial::ipc::UnaryCallReply>(
          std::mem_fn(&RPCService::Service::UnaryCall), this)));
}

RPCService::Service::~Service() {
}

::grpc::Status RPCService::Service::UnaryCall(::grpc::ServerContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace trivial
}  // namespace ipc

