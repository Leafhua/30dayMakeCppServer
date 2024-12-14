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

#include <cstdint>
#include <functional>
#include <sys/epoll.h>

#pragma once

class Socket;
class EventLoop;
class Channel {
private:
  EventLoop *loop;
  int fd;
  uint32_t events;
  uint32_t ready;
  bool inEpoll;
  bool useThreadPool;
  std::function<void()> readCallback;
  std::function<void()> writeCallback;

public:
  Channel(EventLoop *_loop, int _fd);
  ~Channel();

  void handleEvent();
  void enableRead();

  int getFd();

  uint32_t getEvents();
  uint32_t getReady();
  bool getInEpoll();
  void setInEpoll(bool _in = true);
  void useET();

  void setReady(uint32_t);
  void setReadCallback(std::function<void()>);
  void setUseThreadPool(bool use = true);
};