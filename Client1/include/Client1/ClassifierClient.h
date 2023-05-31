#ifndef CLASSIFIERCLIENT_H
#define CLASSIFIERCLIENT_H
#include <thread>

#include <QObject>
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include <proto_out/image.pb.h>
#include <proto_out/image.grpc.pb.h>
#include <Client1/Structures.h>
#include <QImage>

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
        sc::image::Image drawed;
        Status status;
        ClientContext context;
        std::unique_ptr<grpc::ClientAsyncResponseReader<::sc::image::Image>>
            response_reader;
    };

   public:
    explicit ClassifierClient(std::shared_ptr<Channel> channel,
                              QObject* parent = nullptr);

    ~ClassifierClient();

    void Classify(QImage const& imageData);

    void AsyncCompleteRpc();

   signals:
    void newImageRecieved(QImage image, BoundingBox box);

    void newDrawedImageReceived(QImage image);

   private:
    std::unique_ptr<ClassifierService::Stub> stub_;
    CompletionQueue cq_;
    std::thread _t1;

    QImage _tmp;
    std::atomic_bool _shuttingDown = false;
};

#endif  // CLASSIFIERCLIENT_H
