/**
 * @file Acceptor.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once


#include <memory>
#include <functional>

#include "Acceptor.h"
#include "common.h"

class Acceptor {
 public:
  DISALLOW_COPY_AND_MOVE(Acceptor);

  explicit Acceptor(EventLoop *_loop);
  ~Acceptor();

  [[nodiscard]] RC AcceptConnection() const;
  void SetNewConnectionCallback(std::function<void(int)> const &callback);

 private:
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  std::function<void(int)> new_connection_callback_;
};
