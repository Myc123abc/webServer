#ifndef EPOLL_H
#define EPOLL_H

#include <vector>

class Event;

class Epoll {
public:
    Epoll();

    void updateEvent(Event* event);

    std::vector<Event*> wait();

private:
    int epollfd;
};

#endif
