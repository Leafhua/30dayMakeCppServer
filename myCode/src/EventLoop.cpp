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
#include "Poller.h"
#include "ThreadPool.h"

EventLoop::EventLoop() : poller_(new Poller()) {  }

EventLoop::~EventLoop() {
  Quit();
  delete poller_;
}

void EventLoop::Loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    chs = poller_->Poll();
    for (auto &ch : chs) {
      ch->HandleEvent();
    }
  }
}

void EventLoop::Quit() { quit_ = true; };

void EventLoop::UpdateChannel(Channel *_ch) { poller_->UpdateChannel(_ch); }
void EventLoop::DeleteChannel(Channel *_ch) { poller_->DeleteChannel(_ch); }
