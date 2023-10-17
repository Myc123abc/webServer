#include "Server.h"
#include "Event.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventDriven.h"
#include <thread>

Server::Server(EventDriven& ed, int port, size_t num, size_t threads): mainReactor(ed), num(num), threads(threads) {
    acceptor = new Acceptor(mainReactor, port);
    acceptor->setCallback([this](Socket& connfd) { newConnection(connfd); });

    if (num != 0) {
        for (int i = 0; i < num; ++i)
            subReactors.push_back(new EventDriven);
        for (int i = 0; i < num; ++i)
            std::thread([this, i] { subReactors[i]->start(); }).detach();
    }
}

void Server::newConnection(Socket& connfd) {
    int fd = connfd.getFd();
    if (num != 0) {
        Connection* connect = new Connection(*subReactors[fd % subReactors.size()], fd, threads);
        return;
    }
    Connection* connect = new Connection(mainReactor, fd, threads);
}
