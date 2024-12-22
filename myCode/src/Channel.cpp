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

const int Channel::READ_EVENT = 1;
const int Channel::WRITE_EVENT = 2;
const int Channel::ET = 4;

Channel::Channel(EventLoop *_loop, Socket *_socket) : loop_(_loop), socket_(_socket) {}
Channel::~Channel() { loop_->DeleteChannel(this); }

void Channel::HandleEvent() {
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

void Channel::UseET() {
  listen_events_ |= ET;
  loop_->UpdateChannel(this);
}

Socket *Channel::GetSocket() { return socket_; }

int Channel::GetListenEvents() { return listen_events_; }

int Channel::GetReadyEvents() { return ready_events_; }

bool Channel::GetExist() { return exist_; }

void Channel::SetExist(bool _in) { exist_ = _in; }

void Channel::SetReadyEvents(int _ev) {
  if (_ev & READ_EVENT) {
    ready_events_ |= READ_EVENT;
  }
  if (_ev & WRITE_EVENT) {
    ready_events_ |= WRITE_EVENT;
  }
  if (_ev & ET) {
    ready_events_ |= ET;
  }
}

void Channel::SetReadCallback(std::function<void()> const &callback) { read_callback_ = callback; }
void Channel::SetWriteCallback(const std::function<void()> &callback) { write_callback_ = callback; }
