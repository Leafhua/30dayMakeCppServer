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
#include "Channel.h"
#include "Epoll.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) { ep = new Epoll(); }

EventLoop::~EventLoop() { delete ep; }

void EventLoop::loop() {
  while (!quit) {
    std::vector<Channel *> chs;
    chs = ep->poll();
    for (auto it = chs.begin(); it != chs.end(); ++it) {
      (*it)->handleEvent();
    }
  }
}

void EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}