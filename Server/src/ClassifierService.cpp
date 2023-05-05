#include <Server/ClassifierService.h>

namespace DarkNetServer {

grpc::Status ClassifierServiceImpl::classify(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<image::Image, image::Image> *stream) {
    image::Image request;
    while (stream->Read(&request)) {
        // Process the incoming request
        // ...

        // Send a response back to the client
        image::Image response;
        stream->Write(response);
    }
    return grpc::Status::OK;
}

}  // namespace DarkNetServer
