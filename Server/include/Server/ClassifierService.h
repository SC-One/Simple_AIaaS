#ifndef CLASSIFIERSERVICE_H
#define CLASSIFIERSERVICE_H

#include <proto_out/image.pb.h>
#include <proto_out/image.grpc.pb.h>

namespace DarkNetServer {
class ClassifierServiceImpl final : public image::ClassifierService::Service {
   public:
    grpc::Status classify(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<image::Image, image::Image>* stream) override;
};
}  // namespace DarkNetServer

#endif  // CLASSIFIERSERVICE_H
