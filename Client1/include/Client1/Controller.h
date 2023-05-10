#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QImage>
#include <QQmlEngine>
#include <QByteArray>

class Controller : public QObject {
    Q_OBJECT
    struct BoundingBox {
        struct Area {
            int x, y;
            int width, height;
        };

        QString label;
        qreal possibility;
        Area area;
    };

   public:
    explicit Controller(QObject *parent = nullptr);

    static void registerMe();
   public slots:
    void setImage(const uchar *bytes, int size, int format);

   signals:
    void imageCaptured(QImage image);
    void imageFromGRPC(const QByteArray &imageData, BoundingBox label);

   private slots:
    void onImageCaptured(QImage const &image);

   private:
    QImage m_image;
};

#endif  // CONTROLLER_H