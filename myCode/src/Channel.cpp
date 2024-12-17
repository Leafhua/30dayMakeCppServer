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

#include "Epoll.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd)
    : loop_(_loop), fd_(_fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}
Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Channel::HandleEvent() {
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    read_callback_();
  }
  if (ready_events_ & (EPOLLOUT)) {
    write_callback_();
  }
}

void Channel::EnableRead() {
  listen_events_ = EPOLLIN | EPOLLET;
  loop_->UpdateChannel(this);
}

void Channel::UseET() {
  listen_events_ |= EPOLLET;
  loop_->UpdateChannel(this);
}

int Channel::GetFd() { return fd_; }

uint32_t Channel::GetListenEvents() { return listen_events_; }

uint32_t Channel::GetReadyEvents() { return ready_events_; }

bool Channel::GetInEpoll() { return in_epoll_; }

void Channel::SetInEpoll(bool _in) { in_epoll_ = _in; }

void Channel::SetReadyEvents(uint32_t _ev) { ready_events_ = _ev; }

void Channel::SetReadCallback(std::function<void()> _cb) { read_callback_ = _cb; }
