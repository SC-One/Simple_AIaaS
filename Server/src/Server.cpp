#include <Server/Server.h>
#include <Server/ClassifierService.h>
#include <QDebug>

DarkNetServer::Server::Server(const QString &ipPort)
    : _service_impl(new ClassifierServiceImpl), _server(nullptr) {
    _builder.AddListeningPort(ipPort.toStdString(),
                              grpc::InsecureServerCredentials());
    _builder.RegisterService(_service_impl.get());
    _server.reset(_builder.BuildAndStart().release());
    qDebug() << "Server listening on " << ipPort;
}

DarkNetServer::Server::~Server() = default;

void DarkNetServer::Server::wait() { _server->Wait(); }
