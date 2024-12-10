#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() : fd(-1) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  errif(fd == -1, "socket create error");
}
Socket::Socket(int _fd) : fd(_fd) { errif(fd == -1, "socket create error"); }

Socket::~Socket() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}

void Socket::bind(InetAddress *addr) {
  errif(::bind(fd, (sockaddr *)&addr->addr, addr->addr_len) == -1,
        "socket bind error");
}

void Socket::listen() {
  errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
  // 设置非阻塞 优雅的操作，fcntl(fd, F_GETFL)获取fd的状态，O_RDONLY(00) || O_WRONLY(01),之后与O_NONBLOCK(04000)按位或
  // 设置O_RDONLY(00)|O_NONBLOCK(04000) 或者O_WRONLY(01)|O_NONBLOCK(04000)
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
int Socket::accept(InetAddress *addr) {
  int clnt_sockfd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addr_len);
  errif(clnt_sockfd == -1, "socket accept error");
  return clnt_sockfd;
}

int Socket::getFd() { return fd; }