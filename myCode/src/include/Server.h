/**
 * @file Server.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <functional>
#include <map>
#include <vector>
#include "Macros.h"

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
 private:
  EventLoop *main_readctor_;
  Acceptor *acceptor_;
  ThreadPool *thread_pool_;

  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_readctors_;

  std::function<void(Connection *)> on_connect_callback_;

 public:
  explicit Server(EventLoop *);
  ~Server();

  DISALLOW_COPY_AND_MOVE(Server);

  void NewConnection(Socket *sock);
  void DeleteConnection(Socket *sock);
  void OnConnect(std::function<void(Connection *)> fn);
};
