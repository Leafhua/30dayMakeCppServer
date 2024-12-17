/**
 * @file EventLoop.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

EventLoop::EventLoop() { epoll_ = new Epoll(); }

EventLoop::~EventLoop() { delete epoll_; }

void EventLoop::Loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    chs = epoll_->Poll();
    for (auto &ch : chs) {
      ch->HandleEvent();
    }
  }
}

void EventLoop::UpdateChannel(Channel *ch) { epoll_->UpdateChannel(ch); }
