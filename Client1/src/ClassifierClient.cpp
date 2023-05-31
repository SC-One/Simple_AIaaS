#include <Client1/ClassifierClient.h>
#include <thread>
#include <QImage>
#include <QDebug>
#include <QBuffer>
#include <QByteArray>

ClassifierClient::ClassifierClient(std::shared_ptr<Channel> channel,
                                   QObject* parent)
    : QObject{parent}, stub_(ClassifierService::NewStub(channel)) {
    _t1 = std::thread(&ClassifierClient::AsyncCompleteRpc, this);
}

ClassifierClient::~ClassifierClient() {
    _shuttingDown = true;
    cq_.Shutdown();
    _t1.join();
}

void ClassifierClient::Classify(const QImage& imageData) {
    _tmp = imageData;
    Image image;
    // Fill in the image metadata here
    image.set_width(imageData.width());
    image.set_height(imageData.height());
    image.set_channels(imageData.depth() / 8);

    QByteArray imageDataArray;
    QBuffer buffer(&imageDataArray);
    buffer.open(QIODevice::WriteOnly);
    imageData.save(&buffer, "JPG");

    image.set_data(imageDataArray.constData(), imageDataArray.size());

    //    auto x = QImage::fromData(imageDataArray);
    //    qDebug() << x.isNull();

    // ...
    AsyncClientCall* call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncdrawClassifiedImage(&call->context, image, &cq_);
    call->response_reader->StartCall();
    call->response_reader->Finish(&call->drawed, &call->status, (void*)call);
}

void ClassifierClient::AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;
    while (cq_.Next(&got_tag, &ok) && !_shuttingDown) {
        AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
        if (call->status.ok()) {
            // Process the received detected image here
            QImage response;
            auto const bits = call->drawed.data();
            response.loadFromData(reinterpret_cast<const uchar*>(bits.data()),
                                  bits.size(), "JPG");
            emit newDrawedImageReceived(response);
            // ...
        } else {
            std::cout << "RPC failed " << call->status.error_details()
                      << call->status.error_message() << std::endl;
        }
        delete call;
    }
}
