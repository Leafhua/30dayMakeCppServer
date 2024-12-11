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

#include <functional>
#include <sys/epoll.h>

#pragma once

class EventLoop;
class Channel {
private:
  EventLoop *loop;
  int fd;
  u_int32_t events;
  u_int32_t revents;
  bool inEpoll;
  std::function<void()> callback;

public:
  Channel(EventLoop *_loop, int _fd);
  ~Channel();

  void handleEvent();
  void enableReading();

  int getFd();

  uint32_t getEvents();
  uint32_t getRevents();
  bool getInEpoll();
  void setInEpoll();

  void setRevents(uint32_t);
  void setCallback(std::function<void()>);
  ;
};