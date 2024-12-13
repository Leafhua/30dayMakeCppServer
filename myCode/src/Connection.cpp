/**
 * @file Connection.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr) {
  channel = new Channel(loop, sock->getFd());
  auto cb = std::bind(&Connection::echo, this, sock->getFd());
  channel->setCallback(cb);
  channel->enableReading();
}

Connection::~Connection() {
  delete channel;
  delete sock;
}

void Connection::echo(int sockfd) {
  char buf[READ_BUFFER];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      printf("message from client fd %d: %s\n", sockfd, buf);
      write(sockfd, buf, sizeof(buf));

    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        printf("finish reading once, errno: %d\n", errno);
        break;
    } else if (bytes_read == 0) {
        printf("EOF, client fd %d disconnected\n", sockfd);
        deleteConnectionCallback(sock);
        break;
    }
  }
}

void Connection::setDeleteConnectionCallback(std::function<void (Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
