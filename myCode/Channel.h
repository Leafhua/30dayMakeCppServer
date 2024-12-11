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
#include <sys/epoll.h>
#include <sys/types.h>

#pragma once

class Epoll;
class Channel {
private:
  Epoll *ep;
  int fd;
  u_int32_t events;
  u_int32_t revents;
  bool inEpoll;

public:
  Channel(Epoll *_ep, int _fd);
  ~Channel();

  void enableReading();
  int getFd();
  uint32_t getEvents();
  uint32_t getRevents();
  bool getInEpoll();
  void setInEpoll();

  void setRevents(uint32_t);
  ;
};