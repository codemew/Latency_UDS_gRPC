// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: trivial.ipc.proto
#ifndef GRPC_trivial_2eipc_2eproto__INCLUDED
#define GRPC_trivial_2eipc_2eproto__INCLUDED

#include "trivial.ipc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc_impl {
class CompletionQueue;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc_impl

namespace grpc {
namespace experimental {
template <typename RequestT, typename ResponseT>
class MessageAllocator;
}  // namespace experimental
}  // namespace grpc

namespace trivial {
namespace ipc {

class RPCService final {
 public:
  static constexpr char const* service_full_name() {
    return "trivial.ipc.RPCService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::trivial::ipc::UnaryCallReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>> AsyncUnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>>(AsyncUnaryCallRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>> PrepareAsyncUnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>>(PrepareAsyncUnaryCallRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      virtual void UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
    };
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>* AsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::trivial::ipc::UnaryCallReply>* PrepareAsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::trivial::ipc::UnaryCallReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>> AsyncUnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>>(AsyncUnaryCallRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>> PrepareAsyncUnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>>(PrepareAsyncUnaryCallRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)>) override;
      void UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, std::function<void(::grpc::Status)>) override;
      void UnaryCall(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      void UnaryCall(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::trivial::ipc::UnaryCallReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>* AsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::trivial::ipc::UnaryCallReply>* PrepareAsyncUnaryCallRaw(::grpc::ClientContext* context, const ::trivial::ipc::UnaryCallRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_UnaryCall_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status UnaryCall(::grpc::ServerContext* context, const ::trivial::ipc::UnaryCallRequest* request, ::trivial::ipc::UnaryCallReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_UnaryCall() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestUnaryCall(::grpc::ServerContext* context, ::trivial::ipc::UnaryCallRequest* request, ::grpc::ServerAsyncResponseWriter< ::trivial::ipc::UnaryCallReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_UnaryCall<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_UnaryCall() {
      ::grpc::Service::experimental().MarkMethodCallback(0,
        new ::grpc_impl::internal::CallbackUnaryHandler< ::trivial::ipc::UnaryCallRequest, ::trivial::ipc::UnaryCallReply>(
          [this](::grpc::ServerContext* context,
                 const ::trivial::ipc::UnaryCallRequest* request,
                 ::trivial::ipc::UnaryCallReply* response,
                 ::grpc::experimental::ServerCallbackRpcController* controller) {
                   return this->UnaryCall(context, request, response, controller);
                 }));
    }
    void SetMessageAllocatorFor_UnaryCall(
        ::grpc::experimental::MessageAllocator< ::trivial::ipc::UnaryCallRequest, ::trivial::ipc::UnaryCallReply>* allocator) {
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::trivial::ipc::UnaryCallRequest, ::trivial::ipc::UnaryCallReply>*>(
          ::grpc::Service::experimental().GetHandler(0))
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual void UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/, ::grpc::experimental::ServerCallbackRpcController* controller) { controller->Finish(::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "")); }
  };
  typedef ExperimentalWithCallbackMethod_UnaryCall<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_UnaryCall() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_UnaryCall() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestUnaryCall(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_UnaryCall() {
      ::grpc::Service::experimental().MarkMethodRawCallback(0,
        new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this](::grpc::ServerContext* context,
                 const ::grpc::ByteBuffer* request,
                 ::grpc::ByteBuffer* response,
                 ::grpc::experimental::ServerCallbackRpcController* controller) {
                   this->UnaryCall(context, request, response, controller);
                 }));
    }
    ~ExperimentalWithRawCallbackMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual void UnaryCall(::grpc::ServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/, ::grpc::experimental::ServerCallbackRpcController* controller) { controller->Finish(::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "")); }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_UnaryCall : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_UnaryCall() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler< ::trivial::ipc::UnaryCallRequest, ::trivial::ipc::UnaryCallReply>(std::bind(&WithStreamedUnaryMethod_UnaryCall<BaseClass>::StreamedUnaryCall, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_UnaryCall() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status UnaryCall(::grpc::ServerContext* /*context*/, const ::trivial::ipc::UnaryCallRequest* /*request*/, ::trivial::ipc::UnaryCallReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedUnaryCall(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::trivial::ipc::UnaryCallRequest,::trivial::ipc::UnaryCallReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_UnaryCall<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_UnaryCall<Service > StreamedService;
};

}  // namespace ipc
}  // namespace trivial


#endif  // GRPC_trivial_2eipc_2eproto__INCLUDED
