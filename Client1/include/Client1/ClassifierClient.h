#ifndef CLASSIFIERCLIENT_H
#define CLASSIFIERCLIENT_H

#include <QObject>
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include <proto_out/image.pb.h>
#include <proto_out/image.grpc.pb.h>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using sc::image::Area;
using sc::image::ClassifierService;
using sc::image::DetectedImage;
using sc::image::Image;

class ClassifierClient : public QObject {
    Q_OBJECT
    class AsyncClientCall {
       public:
        DetectedImage detected_image;
        Status status;
        ClientContext context;
        std::unique_ptr<grpc::ClientAsyncReaderWriter<Image, DetectedImage>>
            response_reader;
    };

   public:
    explicit ClassifierClient(std::shared_ptr<Channel> channel,
                              QObject* parent = nullptr);

    void Classify(const std::vector<std::vector<uint8_t>>& image_datas) {
        Image image;
        for (const auto& image_data : image_datas) {
            image.set_data(image_data.data(), image_data.size());
            // Fill in the image metadata here
            // ...
            AsyncClientCall* call = new AsyncClientCall;
            auto rel = stub_->PrepareAsyncclassify(&call->context, &cq_);
            auto relRaw = rel.release();
            call->response_reader.reset(relRaw);
            call->response_reader->StartCall((void*)call);
            call->response_reader->Finish(&call->status, (void*)call);
        }
    }

    void AsyncCompleteRpc() {
        void* got_tag;
        bool ok = false;
        while (cq_.Next(&got_tag, &ok)) {
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
            if (call->status.ok()) {
                // Process the received detected image here
                // ...
            } else {
                std::cout << "RPC failed" << std::endl;
            }
            delete call;
        }
    }

   private:
    std::unique_ptr<ClassifierService::Stub> stub_;
    CompletionQueue cq_;
};

// int main(int argc, char** argv) {
//  ClassifierClient client(grpc::CreateChannel("localhost:50051",
//  grpc::InsecureChannelCredentials()));

//  // Prepare the image data to be classified
//  std::vector<std::vector<uint8_t>> image_datas;
//  // ...

//  // Make the async classify call
//  client.Classify(image_datas);

//  // Wait for the response to arrive
//  client.AsyncCompleteRpc();

//  return 0;
//}

#endif  // CLASSIFIERCLIENT_H
