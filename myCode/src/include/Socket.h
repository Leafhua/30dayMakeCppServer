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

#include <netinet/in.h>
#include <cstdint>
#include "Macros.h"
#include "Socket.h"
class InetAddress {
 public:
  InetAddress();
  InetAddress(const char *ip, uint16_t port);
  ~InetAddress() = default;

  DISALLOW_COPY_AND_MOVE(InetAddress);

  void SetAddr(sockaddr_in _addr);
  sockaddr_in GetAddr();
  const char *GetIp();
  uint16_t GetPort();

 private:
  struct sockaddr_in addr_;
};
class Socket {
 private:
  int fd_{-1};

 public:
  Socket();
  explicit Socket(int _fd);
  ~Socket();

  DISALLOW_COPY_AND_MOVE(Socket);
  void Bind(InetAddress *);
  void Listen();
  void Setnonblocking();
  void Connect(InetAddress *);

  int Accept(InetAddress *);

  int GetFd();
};
