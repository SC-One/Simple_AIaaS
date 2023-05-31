#ifndef DETECTEDIMAGEPROVIDER_H
#define DETECTEDIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>
#include <QStandardItemModel>

class DetectedImageProvider final : public QQuickImageProvider {
   public:
    DetectedImageProvider();
    QImage requestImage(const QString &id, QSize *size,
                        const QSize &requestedSize) override;

    QStandardItemModel *model() const;
    void setModel(QStandardItemModel *newModel);

    void setNewImage(int i, QImage const &image);

   private:
    QStandardItemModel *_model;
    QMap<int, QImage> _images;
};

#endif  // DETECTEDIMAGEPROVIDER_H
