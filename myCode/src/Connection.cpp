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
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "util.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_BUFFER 1024

// cpp成员变量声明顺序注意
// 可能导致的问题：如果一个成员变量的初始化依赖于另一个成员变量的值，必须确保依赖的变量在前面声明并初始化。否则，可能会导致未定义行为。
Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr), readBuffer(nullptr),
      inBuffer(new std::string()) {
  channel = new Channel(loop, sock->getFd());
  auto cb = std::bind(&Connection::echo, this, sock->getFd());
  channel->setCallback(cb);
  channel->enableReading();
  readBuffer = new Buffer();
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
      readBuffer->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
      printf("finish reading once\n");
      printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
      errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1,
            "socket wirte error");
      readBuffer->clear();
      break;
    } else if (bytes_read == 0) {
      printf("EOF, client fd %d disconnected\n", sockfd);
      deleteConnectionCallback(sock);
      break;
    }
  }
}

void Connection::setDeleteConnectionCallback(
    std::function<void(Socket *)> _cb) {
  deleteConnectionCallback = _cb;
}
