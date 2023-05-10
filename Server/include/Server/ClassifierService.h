#ifndef CLASSIFIERSERVICE_H
#define CLASSIFIERSERVICE_H

#include <proto_out/image.pb.h>
#include <proto_out/image.grpc.pb.h>

#include <DarknetWrapper/DarkNetWrapper.h>

namespace DarkNetServer {
class ClassifierServiceImpl final
    : public sc::image::ClassifierService::Service {
   public:
    grpc::Status classifyshot(grpc::ServerContext* context,
                              const sc::image::Image* request,
                              sc::image::Detections* response) override;

   private:
    DarknetWrapper _network;
};
}  // namespace DarkNetServer

#endif  // CLASSIFIERSERVICE_H
