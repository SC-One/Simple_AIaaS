#include <QCoreApplication>
#include <Server/Server.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    DarkNetServer::Server server;
    server.wait();
    return a.exec();
}
