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
#include "common.h"
#pragma once

class Channel {
 public:
  DISALLOW_COPY_AND_MOVE(Channel)

  Channel(int _fd, EventLoop *_loop);
  ~Channel();

  void HandleEvent() const;
  void EnableRead();
  void EnableWrite();

  [[nodiscard]] int Fd() const;
  [[nodiscard]] int16_t ListenEvents() const;
  [[nodiscard]] int16_t ReadyEvents() const;
  [[nodiscard]] bool Exist() const;
  void SetExist(bool _in = true);
  void EnableET();

  void SetReadyEvent(int16_t _ev);
  void SetReadCallback(std::function<void()> const &_callback);
  void SetWriteCallback(std::function<void()> const &_callback);

  static const int16_t READ_EVENT;
  static const int16_t WRITE_EVENT;
  static const int16_t EDGE_TRIGGERED;

 private:
  int fd_;
  EventLoop *loop_;
  int16_t listen_events_;
  int16_t ready_events_;
  bool exist_;

  std::function<void()> read_callback_;
  std::function<void()> write_callback_;
};
