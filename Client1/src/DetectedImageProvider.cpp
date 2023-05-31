#include <Client1/DetectedImageProvider.h>
#include <QDir>

DetectedImageProvider::DetectedImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

static const QImage failed(QDir::currentPath() + "/res/failed.png");

QImage DetectedImageProvider::requestImage(const QString &id, QSize *size,
                                           const QSize &requestedSize) {
    // Look up the image in your model based on the ID
    // and return the corresponding QImage.
    bool ok = 0;
    int idNum = id.toInt(&ok);
    if (!ok) return failed;
    auto result = _images[idNum];
    return result;
}

QStandardItemModel *DetectedImageProvider::model() const { return _model; }

void DetectedImageProvider::setModel(QStandardItemModel *newModel) {
    _model = newModel;
}

void DetectedImageProvider::setNewImage(int i, const QImage &image) {
    _images[i] = image;
}
