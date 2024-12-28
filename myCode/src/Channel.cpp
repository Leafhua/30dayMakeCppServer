/**
 * @file Channel.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Channel.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <cstdint>

#include "EventLoop.h"
#include "Socket.h"

const int16_t Channel::READ_EVENT = 1;
const int16_t Channel::WRITE_EVENT = 2;
const int16_t Channel::EDGE_TRIGGERED = 4;

Channel::Channel(int _fd, EventLoop *_loop)
    : fd_(_fd), loop_(_loop), listen_events_(0), ready_events_(0), exist_(false) {}

Channel::~Channel() { loop_->DeleteChannel(this); }

void Channel::HandleEvent() const {
  if (ready_events_ & READ_EVENT) {
    read_callback_();
  }
  if (ready_events_ & WRITE_EVENT) {
    write_callback_();
  }
}

void Channel::EnableRead() {
  listen_events_ = READ_EVENT;
  loop_->UpdateChannel(this);
}

void Channel::EnableWrite() {
  listen_events_ = WRITE_EVENT;
  loop_->UpdateChannel(this);
}

void Channel::EnableET() {
  listen_events_ |= EDGE_TRIGGERED;
  loop_->UpdateChannel(this);
}

int Channel::Fd() const { return fd_; }

int16_t Channel::ListenEvents() const { return listen_events_; }

int16_t Channel::ReadyEvents() const { return ready_events_; }

bool Channel::Exist() const { return exist_; }

void Channel::SetExist(bool _in) { exist_ = _in; }

void Channel::SetReadyEvent(int16_t _ev) {
  if (_ev & READ_EVENT) {
    ready_events_ |= READ_EVENT;
  }
  if (_ev & WRITE_EVENT) {
    ready_events_ |= WRITE_EVENT;
  }
  if (_ev & EDGE_TRIGGERED) {
    ready_events_ |= EDGE_TRIGGERED;
  }
}

void Channel::SetReadCallback(std::function<void()> const &callback) { read_callback_ = callback; }
void Channel::SetWriteCallback(const std::function<void()> &callback) { write_callback_ = callback; }
