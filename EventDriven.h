#ifndef EVENTDRIVEN_H
#define EVENTDRIVEN_H

#include "Epoll.h"
#include <functional>

class Event;

class EventDriven {
public:
    void start();
    void updateEvent(Event* event) { epollfd.updateEvent(event); }

private:
    Epoll       epollfd; 
};

#endif
