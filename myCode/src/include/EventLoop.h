/**
 * @file EventLoop.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <memory>
#include "common.h"
class EventLoop {
 public:
  DISALLOW_COPY_AND_MOVE(EventLoop);

  EventLoop();
  ~EventLoop();
  void Loop() const;
  void UpdateChannel(Channel *_ch) const;
  void DeleteChannel(Channel *_ch) const;

 private:
  std::unique_ptr<Poller> poller_;
};
