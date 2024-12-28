/**
 * @file Acceptor.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Acceptor.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>

#include <cstdio>
#include <functional>
#include <memory>
#include <utility>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "common.h"

Acceptor::Acceptor(EventLoop *_loop) {
  socket_ = std::make_unique<Socket>();
  assert(socket_->Create() == RC_SUCCESS);
  assert(socket_->Bind("127.0.0.1", 8883) == RC_SUCCESS);
  assert(socket_->Listen() == RC_SUCCESS);

  channel_ = std::make_unique<Channel>(socket_->Fd(), _loop);
  std::function<void()> callback = std::bind(&Acceptor::AcceptConnection, this);

  channel_->SetReadCallback(callback);
  channel_->EnableRead();
}

Acceptor::~Acceptor() = default;

RC Acceptor::AcceptConnection() const {
  int clnt_fd = -1;
  if (socket_->Accept(clnt_fd) != RC_SUCCESS) {
    return RC_ACCEPTOR_ERROR;
  }

  // TODO(hua): remove
  fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);  // 新接受到的连接设置为非阻塞式
  if (new_connection_callback_) {
    new_connection_callback_(clnt_fd);
  }

  return RC_SUCCESS;
}

void Acceptor::SetNewConnectionCallback(std::function<void(int)> const &callback) {
  new_connection_callback_ = callback;
}
