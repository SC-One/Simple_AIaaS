#include <Client1/ClassifierClient.h>
#include <thread>
#include <QImage>

ClassifierClient::ClassifierClient(std::shared_ptr<Channel> channel,
                                   QObject* parent)
    : QObject{parent}, stub_(ClassifierService::NewStub(channel)) {
    _t1 = std::thread(&ClassifierClient::AsyncCompleteRpc, this);
}

ClassifierClient::~ClassifierClient() { _t1.join(); }

void ClassifierClient::Classify(const QImage& imageData) {
    _tmp = imageData;
    Image image;
    // Fill in the image metadata here
    std::vector<char> x;
    image.set_data(x.data(), x.size());
    // ...
    AsyncClientCall* call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncclassifyshot(&call->context, image, &cq_);
    call->response_reader->StartCall();
    call->response_reader->Finish(&call->detections, &call->status,
                                  (void*)call);
}

void ClassifierClient::AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;
    while (cq_.Next(&got_tag, &ok)) {
        AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
        if (call->status.ok()) {
            // Process the received detected image here
            QImage response;
            //             the replay: call->detections;
            //            response.fromData();
            int maxPosIndex = -1;
            int maxPos = -1;
            for (std::size_t i = 0; i < call->detections.detections_size();
                 ++i) {
                auto detect = call->detections.detections(i);
                if (detect.possibility() > maxPos) {
                    maxPos = detect.possibility();
                    maxPosIndex = i;
                }
            }

            auto const bestDetect = call->detections.detections(maxPosIndex);
            BoundingBox box;
            box.area.x = bestDetect.area().x();
            box.area.x = bestDetect.area().y();
            box.area.width = bestDetect.area().w();
            box.area.height = bestDetect.area().h();
            box.label = QString::fromStdString(bestDetect.label());
            box.possibility = bestDetect.possibility();
            emit newImageRecieved(_tmp, box);
            // ...
        } else {
            std::cout << "RPC failed" << std::endl;
        }
        delete call;
    }
}
