#include "Event.h"
#include "EventDriven.h"
#include <sys/epoll.h>
#include <unistd.h>

#define EPOLLOTH    EPOLLRDHUP | EPOLLET | EPOLLONESHOT

void Event::enableListening() {
    event = EPOLLIN;
    ed.updateEvent(this);
}

void Event::enableReading() {
    event = EPOLLIN | EPOLLOTH;
    ed.updateEvent(this);
}

void Event::enableWriting() {
    event = EPOLLOUT | EPOLLOTH;
    ed.updateEvent(this);
}

void Event::close() {
    ::close(fd);
    delete this;
}

void Event::handleEvent() {
    callback();
}
