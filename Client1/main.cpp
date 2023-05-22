#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>
#include <Client1/Controller.h>
#include <Client1/DetectedImageProvider.h>
#include <QScopedPointer>
#include <QQmlContext>
#include <Client1/ClassifierClient.h>

static QScopedPointer<QStandardItemModel> imageModel(new QStandardItemModel());

int main(int argc, char *argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    {
        Controller::registerMe();  // exposing in qml
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Client1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    Controller mainController;

    ///@@@on recieving image model we should insert it in model
    QImage image;
    QString id;
    QStandardItem *item = new QStandardItem();
    item->setData(image, Qt::DecorationRole);  //
    item->setData(id, Qt::UserRole);
    imageModel->appendRow(item);
    ///!!!!!!!!!!!!!!!!!!

    engine.rootContext()->setContextProperty("mainController", &mainController);
    {
        {
            // Network

            ClassifierClient client(grpc::CreateChannel(
                "localhost:50051", grpc::InsecureChannelCredentials()));
        }
        DetectedImageProvider imageProvider;
        imageProvider.setModel(imageModel.get());
        engine.addImageProvider("myImageProvider", &imageProvider);
        engine.rootContext()->setContextProperty("detectedImagesModel",
                                                 imageModel.get());
    }

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
