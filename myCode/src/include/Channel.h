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
  Channel(EventLoop *_loop, Socket *_socket);
  ~Channel();

  DISALLOW_COPY_AND_MOVE(Channel)
  void HandleEvent();
  void EnableRead();
  void EnableWrite();

  Socket *GetSocket();

  int GetListenEvents();
  int GetReadyEvents();


  bool GetExist();
  void SetExist(bool _in = true);

  void UseET();

  void SetReadyEvents(int _ev);
  void SetReadCallback(std::function<void()> const &callback);
  void SetWriteCallback(std::function<void()> const &callback);

  static const int READ_EVENT;
  static const int WRITE_EVENT;
  static const int ET;

 private:
  EventLoop *loop_;
  Socket *socket_;
  int listen_events_{0};
  int ready_events_{0};
  bool exist_{false};
  std::function<void()> read_callback_;
  std::function<void()> write_callback_;
};
