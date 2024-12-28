/**
 * @file TcpServer.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "common.h"

class TcpServer {
 public:
  DISALLOW_COPY_AND_MOVE(TcpServer);
  TcpServer();
  ~TcpServer();

  void Start();
  RC NewConnection(int _fd);
  RC DeleteCOnnection(int _fd);

  void OnConnect(std::function<void(Connection *)> _fn);
  void OnRecv(std::function<void(Connection *)> _fn);

 private:
  std::unique_ptr<EventLoop> main_reactor_;
  std::unique_ptr<Acceptor> acceptor_;

  std::unordered_map<int, std::unique_ptr<Connection>> connections_;
  std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

  std::unique_ptr<ThreadPool> thread_pool_;

  std::function<void(Connection *)> on_connect_;
  std::function<void(Connection *)> on_recv_;
};
