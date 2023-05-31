#include <Client1/Controller.h>
#include <QImage>
#include <QPixmap>
#include <Client1/ClassifierClient.h>
#include <QDebug>
#include <QUrl>
#include <QCamera>


Controller::Controller(QObject *parent) :
    QObject{parent},
    _qCamera(nullptr),
    m_capture(nullptr)
{
    connect(this, &Controller::imageCaptured, this,
            &Controller::onImageCaptured, Qt::DirectConnection);
}

void Controller::setNetwork(QWeakPointer<ClassifierClient> classifier) {
    disconnect(_classifierClient.lock().get());
    _classifierClient = classifier;
    auto shared = _classifierClient.lock();
    if (!shared.isNull()) {
        connect(shared.get(), &ClassifierClient::newDrawedImageReceived, this,
                [this](QImage const &image) {
                    qDebug() << image.isNull() << image.bits();
                    qDebug() << image.sizeInBytes();
                    emit drawedImageFromGRPC(image);
                });
    }
}

void Controller::registerMe() {
    qmlRegisterType<Controller>("AIaaS.Simple.Client", 1, 0, "Controller");
}

void Controller::setImage(const uchar *bytes, int size, int format) {
    m_image = QImage::fromData(bytes, size, nullptr);
    onImageCaptured(m_image);
    //    emit imageCaptured(m_image);
}

void Controller::setImageCaptured(const QImage &image)
{
    m_image = image;
    onImageCaptured(m_image);
}

void Controller::setImagePath(const QString &path) {
    m_image = QImage(QUrl(path).toLocalFile());
    onImageCaptured(m_image);
    //    emit imageCaptured(m_image);
}

void Controller::onImageCaptured(const QImage image) {
    auto shared = _classifierClient.lock();
    if (!shared.isNull()) {
        shared->Classify(image);
    }
}


void Controller::setQCamera(QObject *qCamera)
{
    if (_qCamera == qCamera)
        return;
    if(!m_capture.isNull()) {
        m_capture.take()->deleteLater();
    }

    if(_qCamera)
        _qCamera->disconnect();
    _qCamera = qCamera;

    if(QCamera *camera = qvariant_cast<QCamera *>(_qCamera->property("mediaObject"))){
        m_capture.reset(new QCameraImageCapture(camera, this));
        m_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        connect(m_capture.get(), &QCameraImageCapture::imageCaptured, this,
                [this]([[maybe_unused]] int id, QImage const &preview){
            this->setImageCaptured(preview);
        });
    }
    else
        m_capture.reset();
}
