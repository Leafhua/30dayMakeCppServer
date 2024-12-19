/**
 * @file EventLoop.h
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
#include "Macros.h"
class ThreadPool;
class Epoll;
class Channel;
class EventLoop {
 public:
  EventLoop();
  ~EventLoop();
  DISALLOW_COPY_AND_MOVE(EventLoop);
  void Loop();
  void UpdateChannel(Channel *);


 private:
  Epoll *epoll_{nullptr};
  bool quit_{false};
};
