#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Socket.h"
#include <functional>

class EventDriven;
class Event;

class Acceptor {
public:
    Acceptor(EventDriven& ed, int port);

    void setCallback(std::function<void(Socket&)> callback);

    void acceptConnection();

private:
    Socket listenfd;
    Event* event;
    std::function<void(Socket&)> callback;
};

#endif
