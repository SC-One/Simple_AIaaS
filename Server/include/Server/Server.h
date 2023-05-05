#ifndef MY_SERVER_H
#define MY_SERVER_H

#include <QString>
#include <QScopedPointer>
#include <grpcpp/grpcpp.h>

namespace DarkNetServer {
class ClassifierServiceImpl;

class Server final {
   public:
    Server(QString const &ipPort = "0.0.0.0:50777");
    ~Server();
    ///
    /// \brief you shoudl call this after configured the server(for now ist just
    /// constructing)
    ///
    void wait();

   private:
    QScopedPointer<ClassifierServiceImpl> _service_impl;
    QScopedPointer<grpc::Server> _server;
    grpc::ServerBuilder _builder;
};

}  // namespace DarkNetServer

#endif  // MY_SERVER_H
