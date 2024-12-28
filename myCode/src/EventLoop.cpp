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
#include <memory>
#include <vector>
#include "Channel.h"
#include "Poller.h"
#include "ThreadPool.h"

EventLoop::EventLoop() { poller_ = std::make_unique<Poller>(); }

EventLoop::~EventLoop() = default;

void EventLoop::Loop() const {
  while (true) {
    for (Channel *active_ch : poller_->Poll()) {
      active_ch->HandleEvent();
    }
  }
}

void EventLoop::UpdateChannel(Channel *_ch) const { poller_->UpdateChannel(_ch); }
void EventLoop::DeleteChannel(Channel *_ch) const { poller_->DeleteChannel(_ch); }
