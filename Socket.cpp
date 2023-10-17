#include "Socket.h"
#include "error.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

void Socket::setnonblocking() { fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }

Socket::Socket() {
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket create error");
}

Socket& Socket::reUseAddr() {
    int tmp = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp)) < 0)
        error("set SO_REUSEADDR error");
    return *this;
}

Socket& Socket::bind(int port) {
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);
    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        error("socket bind error");
    return *this;
}

Socket& Socket::listen() {
    if (::listen(fd, SOMAXCONN) < 0)
        error("socket listen error");
    return *this;
}

int Socket::accept() {
    int connfd;
    if ((connfd = ::accept(fd, NULL, NULL)) < 0)
        error("socket accept error");
    return connfd;
}
