#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QImage>
#include <QQmlEngine>

class Controller : public QObject {
    Q_OBJECT
   public:
    explicit Controller(QObject *parent = nullptr);

    static void registerMe();
   public slots:
    void setImage(const uchar *bytes, int size, int format);

   signals:
    void imageCaptured(QImage image);

   private slots:
    void onImageCaptured(QImage const &image);

   private:
    QImage m_image;
};

#endif  // CONTROLLER_H
