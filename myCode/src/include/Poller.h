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
#include "common.h"

#ifdef OS_LINUX
#include "sys/epoll.h"
#endif

#ifdef OS_MACOS
#include "sys/event.h"
#endif
class Channel;
class Poller {
 public:
  DISALLOW_COPY_AND_MOVE(Poller);

  Poller();
  ~Poller();

  RC UpdateChannel(Channel *_ch) const;
  RC DeleteChannel(Channel *_ch) const;

  [[nodiscard]] std::vector<Channel *> Poll(int timeout = -1) const;

 private:
  int fd_;

#ifdef OS_LINUX
  struct epoll_event *events_{nullptr};
#endif

#ifdef OS_MACOS
  struct kevent *events_{nullptr};
#endif
};
