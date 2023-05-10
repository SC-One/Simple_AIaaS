#include <Client1/DetectedImageProvider.h>

DetectedImageProvider::DetectedImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage DetectedImageProvider::requestImage(const QString &id, QSize *size,
                                           const QSize &requestedSize) {
    // Look up the image in your model based on the ID
    // and return the corresponding QImage.
}

QStandardItemModel *DetectedImageProvider::model() const { return _model; }

void DetectedImageProvider::setModel(QStandardItemModel *newModel) {
    _model = newModel;
}
