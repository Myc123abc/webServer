#include "Connection.h"
#include "Event.h"
#include "error.h"
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std::literals::chrono_literals;

Connection::Connection(EventDriven& ed, int fd, size_t num): num(num) {
    pool = initThreadPool(num);

    event = new Event(ed, fd);
    event->setCallback([this] { handleReadEvent(); });
    event->enableReading();
}

int Connection::checkErr() {
    if (event->getEvent() & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        event->close();
        delete this;
        return -1;
    }
    return 0;
}
void Connection::handleReadEvent() {
    if (checkErr() < 0)
        return;

    while (true) {
        int bytes_read;
        if ((bytes_read = recv(event->getFd(), recvbuf, sizeof(recvbuf), 0)) < 0) {
            if (errno == EAGAIN) {
                break;
            }
            error("recv error");
        }
        if (bytes_read == 0) {
            event->close();
            delete this;
            return;
        }
    }
    
    if (num != 0) {
        pool->enqueue([this] { process(); });
        return;
    } else
        process();
}

void Connection::process() {
    strcpy(sendbuf, "HTTP/1.1 200 OK\r\n\r\n");
    strcat(sendbuf, "<html>");
    strcat(sendbuf, recvbuf);
    strcat(sendbuf, "</html>");

    std::this_thread::sleep_for(500us);

    event->setCallback([this] { handleWriteEvent(); });
    event->enableWriting();
}

void Connection::handleWriteEvent() {
    ssize_t bytes_write = 0;
    while (true) {
        bytes_write += send(event->getFd(), sendbuf + bytes_write, strlen(sendbuf) - bytes_write, MSG_NOSIGNAL);
        if (bytes_write == strlen(sendbuf))
            break;
        if (bytes_write < 0) {
            if (errno == EAGAIN) {
                event->enableWriting();
                return;
            }
            if (errno == EPIPE)
                break;
            error("send error");
        }
    }
    event->close();
    delete this;
}
