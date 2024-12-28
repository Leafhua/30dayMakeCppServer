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

#include <cstddef>
#include <cstdint>
#include <string>
#include "common.h"
class Socket {
 private:
  int fd_;

 public:
  DISALLOW_COPY_AND_MOVE(Socket);

  Socket();
  ~Socket();

  void SetFd(int _fd);
  [[nodiscard]] int Fd() const;
  [[nodiscard]] std::string GetAddr() const;

  RC Create();
  RC Bind(const char *_ip, uint16_t _port) const;
  [[nodiscard]] RC Listen() const;
  RC Accept(int &_clnt_fd) const;
  RC Connect(const char *_ip, uint16_t _port) const;
  [[nodiscard]] RC SetNonBlocking() const;

  [[nodiscard]] bool IsNonBlocking() const;
  [[nodiscard]] size_t RecvBufSize() const;
};
