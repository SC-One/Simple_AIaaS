#include <Server/ClassifierService.h>

namespace DarkNetServer {

grpc::Status ClassifierServiceImpl::classify(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<sc::image::DetectedImage, sc::image::Image>*
        stream) {
    sc::image::Image request;
    while (stream->Read(&request)) {
        // Process the incoming request
        // ...
        auto x = request.data();

        // Send a response back to the client
        sc::image::DetectedImage response;
        stream->Write(response);
    }
    return grpc::Status::OK;
}

}  // namespace DarkNetServer
