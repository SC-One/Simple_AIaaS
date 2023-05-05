#ifndef CLASSIFIERSERVICE_H
#define CLASSIFIERSERVICE_H

#include <proto_out/image.pb.h>
#include <proto_out/image.grpc.pb.h>

#include <DarknetWrapper/DarkNetWrapper.h>

namespace DarkNetServer {
class ClassifierServiceImpl final
    : public sc::image::ClassifierService::Service {
   public:
    grpc::Status classify(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<sc::image::DetectedImage, sc::image::Image>*
            stream) override;
};
}  // namespace DarkNetServer

#endif  // CLASSIFIERSERVICE_H
