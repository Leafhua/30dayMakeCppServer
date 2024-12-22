/**
 * @file Poller.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <vector>
#include "Macros.h"

#ifdef OS_LINUX
#include "sys/epoll.h"
#endif

#ifdef OS_MACOS
#include "sys/event.h"
#endif
class Channel;
class Poller {
 public:
  Poller();
  ~Poller();

  DISALLOW_COPY_AND_MOVE(Poller);

  void UpdateChannel(Channel *_ch);
  void DeleteChannel(Channel *_ch);

  std::vector<Channel *> Poll(int timeout = -1);

 private:
  int fd_{1};


#ifdef OS_LINUX
  struct epoll_event *events_{nullptr};
#endif

#ifdef OS_MACOS
  struct kevent *events_{nullptr};
#endif

};