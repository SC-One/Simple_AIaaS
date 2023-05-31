#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>

#include <QLocale>
#include <QTranslator>
#include <Client1/Controller.h>
#include <Client1/DetectedImageProvider.h>
#include <QScopedPointer>
#include <QQmlContext>
#include <Client1/ClassifierClient.h>

int main(int argc, char *argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setOrganizationName("SC-One");
    QCoreApplication::setApplicationName("Simple_AIaaS");
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

    engine.rootContext()->setContextProperty("detectedImagesModel", 0);
    engine.rootContext()->setContextProperty("mainController", &mainController);
    QSharedPointer<ClassifierClient> client(
        new ClassifierClient(grpc::CreateChannel(
            "localhost:50777", grpc::InsecureChannelCredentials())));
    DetectedImageProvider imageProvider;
    {
        mainController.setNetwork(client);

        engine.addImageProvider("myImageProvider", &imageProvider);
        QObject::connect(&mainController, &Controller::drawedImageFromGRPC,
                         &mainController, [&](QImage const &image) {
                             static int i = -1;
                             ++i;
                             imageProvider.setNewImage(i, image);
                             engine.rootContext()->setContextProperty(
                                 "detectedImagesModel", i + 1);
                         });
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
