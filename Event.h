#ifndef EVENT_H
#define EVENT_H

#include <functional>

class EventDriven;

class Event {
public:
    Event(EventDriven& ed, int fd): ed(ed), fd(fd), inEpoll(false) {}

    void enableListening();
    void enableReading();
    void enableWriting();
    void close();

    bool getInEpoll() { return inEpoll; }
    void setInEpoll() { inEpoll = true; }

    int  getEvent()   { return event; }
    void setEvent(int event) { this->event = event; }

    int  getFd()      { return fd; }

    void handleEvent();
    void setCallback(std::function<void()> cb) { callback = cb; }

private:
    EventDriven& ed;
    int     fd;
    int     event;
    bool    inEpoll;
    std::function<void()> callback;
};

#endif
