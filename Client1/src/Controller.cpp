#include <Client1/Controller.h>

Controller::Controller(QObject *parent) : QObject{parent} {
    connect(this, &Controller::imageCaptured, this,
            &Controller::onImageCaptured);
}

void Controller::registerMe() {
    qmlRegisterType<Controller>("AIaaS.Simple.Client", 1, 0, "Controller");
}

void Controller::setImage(const uchar *bytes, int size, int format) {
    m_image = QImage::fromData(bytes, size, nullptr);
    emit imageCaptured(m_image);
}

void Controller::onImageCaptured(const QImage &image) {
    // I will fill here with sending to server
}
