/**
 * @file Socket.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Socket.h"
class InetAddress;
class Socket {
private:
  int fd;

public:
  Socket();
  Socket(int _fd);

  ~Socket();
  void bind(InetAddress *);
  void listen();
  void setnonblocking();

  int accept(InetAddress *);

  int getFd();
};
