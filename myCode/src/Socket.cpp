#include "Socket.h"
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include "common.h"
#include "util.h"

Socket::Socket() : fd_(-1) {}

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Socket::SetFd(int _fd) { fd_ = _fd; }

int Socket::Fd() const { return fd_; }

std::string Socket::GetAddr() const {
  struct sockaddr_in addr {};
  memset(&addr, 0, sizeof(addr));
  socklen_t len = sizeof(addr);
  if (getpeername(fd_, (struct sockaddr *)&addr, &len) == -1) {
    return "";
  }
  char ipstr[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &addr.sin_addr, ipstr, sizeof(ipstr)) == nullptr) {
    return "";
  }

  std::string ret(ipstr);
  ret += ":";
  ret += std::to_string(htons(addr.sin_port));
  return ret;
}

RC Socket::SetNonBlocking() const {
  // 设置非阻塞 优雅的操作，fcntl(fd, F_GETFL)获取fd的状态，O_RDONLY(00) || O_WRONLY(01),之后与O_NONBLOCK(04000)按位或
  // 设置O_RDONLY(00)|O_NONBLOCK(04000) 或者O_WRONLY(01)|O_NONBLOCK(04000)
  if (fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK) == -1) {
    perror("Socket set nonblocking failed");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}

bool Socket::IsNonBlocking() const { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

size_t Socket::RecvBufSize() const {
  size_t size = -1;
  if (ioctl(fd_, FIONREAD, &size) == -1) {
    perror("Socket get recv buf size failed");
  }
  return size;
}

RC Socket::Create() {
  assert(fd_ == -1);
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ == -1) {
    perror("Failed to create socket");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}

RC Socket::Bind(const char *_ip, uint16_t _port) const {
  assert(fd_ != -1);
  struct sockaddr_in addr {};
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(_ip);
  addr.sin_port = htons(_port);
  if (::bind(fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Failed to bind socket");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}

RC Socket::Listen() const {
  assert(fd_ != -1);
  if (::listen(fd_, SOMAXCONN) == -1) {
    perror("Failed to listen socket");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}

RC Socket::Accept(int &clnt_fd) const {
  // TODO(HUA) non-blocking
  assert(fd_ != -1);
  clnt_fd = ::accept(fd_, nullptr, nullptr);
  if (clnt_fd == -1) {
    perror("Failed to accept socket");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}

RC Socket::Connect(const char *_ip, uint16_t _port) const {
  // TODO(HUA) non-blocking

  struct sockaddr_in addr {};
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(_ip);
  addr.sin_port = htons(_port);
  if (::connect(fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Failed to connect socket");
    return RC_SOCKET_ERROR;
  }
  return RC_SUCCESS;
}
