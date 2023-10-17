#ifndef CONNECTION_H
#define CONNECTION_H

#include <stddef.h>

#define BUFFER_SIZE 1024

class EventDriven;
class Event;
class ThreadPool;

class Connection {
public:
    Connection(EventDriven& ed, int fd, size_t num);

    int  checkErr();
    void handleReadEvent();
    void process();
    void handleWriteEvent();

private:
    Event* event;
    char recvbuf[BUFFER_SIZE];
    char sendbuf[BUFFER_SIZE];
    ThreadPool* pool;
    size_t num;
};

#endif
