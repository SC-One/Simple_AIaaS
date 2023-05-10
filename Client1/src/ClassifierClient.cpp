#include <Client1/ClassifierClient.h>

ClassifierClient::ClassifierClient(std::shared_ptr<Channel> channel,
                                   QObject* parent)
    : QObject{parent}, stub_(ClassifierService::NewStub(channel)) {}
