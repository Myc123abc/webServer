#include "Acceptor.h"
#include "Event.h"

Acceptor::Acceptor(EventDriven& ed, int port) {
    listenfd.reUseAddr().bind(port).listen();
    event = new Event(ed, listenfd.getFd());
    event->setCallback([this] { acceptConnection(); });
    event->enableListening();
}

void Acceptor::setCallback(std::function<void(Socket&)> callback) { 
    this->callback = callback;
}

void Acceptor::acceptConnection() {
    Socket connfd(listenfd.accept());
    connfd.setnonblocking();
    callback(connfd);
}
