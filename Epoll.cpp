#include "Epoll.h"
#include "error.h"
#include "Event.h"
#include <sys/epoll.h>

#define EVENTS  1000

Epoll::Epoll() {
    if ((epollfd = epoll_create1(0)) < 0)
        error("epoll create error");
}

void Epoll::updateEvent(Event* event) {
    struct epoll_event ev;
    ev.data.ptr = event;
    ev.events   = event->getEvent(); 
    if (!event->getInEpoll()) {
        event->setInEpoll();
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, event->getFd(), &ev) < 0)
            error("epoll add error");
    } else {
        if (epoll_ctl(epollfd, EPOLL_CTL_MOD, event->getFd(), &ev) < 0)
            error("epoll mod error");
    }
}

std::vector<Event*> Epoll::wait() {
    struct epoll_event evs[EVENTS];
    int num;
    if ((num = epoll_wait(epollfd, evs, EVENTS, -1)) < 0)
        error("epoll_wait error");

    std::vector<Event*> ret;
    for (int i = 0; i < num; ++i) {
        Event* event = static_cast<Event*>(evs[i].data.ptr);
        event->setEvent(evs[i].events);
        ret.push_back(event); 
    }
    return ret;
}
