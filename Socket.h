#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
    Socket();
    Socket(int fd): fd(fd) {}

    Socket& reUseAddr();
    Socket& bind(int port);
    Socket& listen();
    int     accept();

    void setnonblocking();

    int getFd() { return fd; }

private:
    int fd;
};

#endif
