/**
 * @file Channel.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <sys/epoll.h>
#include <cstdint>
#include <functional>
#include "Macros.h"

#pragma once

class Socket;
class EventLoop;
class Channel {
 public:
  Channel(EventLoop *_loop, int _fd);
  ~Channel();

  DISALLOW_COPY_AND_MOVE(Channel)
  void HandleEvent();
  void EnableRead();

  int GetFd();

  uint32_t GetListenEvents();
  uint32_t GetReadyEvents();
  bool GetInEpoll();
  void SetInEpoll(bool _in = true);
  void UseET();

  void SetReadyEvents(uint32_t);
  void SetReadCallback(std::function<void()>);

 private:
  EventLoop *loop_;
  int fd_;
  uint32_t listen_events_;
  uint32_t ready_events_;
  bool in_epoll_;
  std::function<void()> read_callback_;
  std::function<void()> write_callback_;
};
