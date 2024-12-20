/**
 * @file Acceptor.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Macros.h"

#include <functional>
class EventLoop;
class Socket;
class Channel;
class Acceptor {
 public:
  explicit Acceptor(EventLoop *_loop);
  ~Acceptor();

  DISALLOW_COPY_AND_MOVE(Acceptor);

  void AcceptConnection();
  void SetNewConnectionCallback(std::function<void(Socket *)>);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_;
  std::function<void(Socket *)> new_connection_callback_;
};
