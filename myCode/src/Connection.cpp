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

#include <unistd.h>

#include <cstring>
#include <iostream>
#include <utility>

#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"

#define READ_BUFFER 1024

// cpp成员变量声明顺序注意
// 可能导致的问题：如果一个成员变量的初始化依赖于另一个成员变量的值，必须确保依赖的变量在前面声明并初始化。否则，可能会导致未定义行为。
Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop_(_loop), sock_(_sock), channel_(nullptr), read_buffer_(nullptr) {
  channel_ = new Channel(loop_, sock_->GetFd());
  channel_->EnableRead();
  channel_->UseET();
  auto cb = std::bind(&Connection::Echo, this, sock_->GetFd());
  channel_->SetReadCallback(cb);
  read_buffer_ = new Buffer();
}

Connection::~Connection() {
  delete channel_;
  delete sock_;
  delete read_buffer_;
}

void Connection::Echo(int sockfd) {
  char buf[READ_BUFFER];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buffer_->Append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
      // printf("finish reading once\n");
      printf("message from client fd %d: %s\n", sockfd, read_buffer_->ToStr());
      // errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1,
      //       "socket wirte error");
      Send(sockfd);
      read_buffer_->Clear();
      break;
    } else if (bytes_read == 0) {
      printf("EOF, client fd %d disconnected\n", sockfd);
      delete_connectioin_callback_(sockfd);
      break;
    } else {
      printf("Connection reset by peer\n");
      delete_connectioin_callback_(sockfd);
      break;
    }
  }
}

void Connection::SetDeleteConnectionCallback(std::function<void(int)> const &callback) {
  delete_connectioin_callback_ = callback;
}

void Connection::Send(int sockfd) {
  char buf[read_buffer_->Size()];
  snprintf(buf, sizeof(buf), "%s", read_buffer_->ToStr());
  // strcpy(buf, read_buffer_->ToStr());
  int data_size = read_buffer_->Size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    data_left -= bytes_write;
  }
}
