#ifndef SERVER_H
#define SERVER_H

#include <vector>

class EventDriven;
class Socket;
class Event;
class Acceptor;

class Server {
public:
    Server(EventDriven& ed, int port, size_t num, size_t threads);

    void newConnection(Socket& listenfd);

private:
    EventDriven&                mainReactor; 
    std::vector<EventDriven*>   subReactors;
    Acceptor*       acceptor;
    size_t num;
    size_t threads;
};

#endif
