#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QImage>
#include <QQmlEngine>
#include <QByteArray>
#include <Client1/Structures.h>
#include <QWeakPointer>
#include<QScopedPointer>
#include <QCameraImageCapture>


class QCamera;
class ClassifierClient;

class Controller : public QObject {
    Q_OBJECT
   public:
    explicit Controller(QObject *parent = nullptr);

    void setNetwork(QWeakPointer<ClassifierClient> classifier);
    static void registerMe();

    Q_INVOKABLE void setQCamera(QObject *qCamera);

public slots:
    void setImage(const uchar *bytes, int size, int format);
    void setImageCaptured(QImage const & image);
    void setImagePath(QString const &path);

   signals:
    void imageCaptured(QImage image);
    void imageFromGRPC(const QByteArray &imageData, BoundingBox label);
    void drawedImageFromGRPC(const QImage);

   private slots:
    void onImageCaptured(const QImage image);

   private:
    QImage m_image;
    QWeakPointer<ClassifierClient> _classifierClient;
    QObject *_qCamera;
    QScopedPointer<QCameraImageCapture> m_capture;
};

#endif  // CONTROLLER_H
