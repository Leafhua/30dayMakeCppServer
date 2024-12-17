/**
 * @file Connection.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <string>
#include "Macros.h"
class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection {
 public:
  Connection(EventLoop *_loop, Socket *_sock);
  ~Connection();
  DISALLOW_COPY_AND_MOVE(Connection)

  void Echo(int sockfd);
  void SetDeleteConnectionCallback(std::function<void(int)> const &callback);
  void Send(int sockfd);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_;
  Buffer *read_buffer_;
  std::function<void(int)> delete_connectioin_callback_;
};
