/**
 * @file Poller.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Poller.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include "Channel.h"
#include "Socket.h"
#include "common.h"
#include "util.h"

#define MAX_EVENTS 1024

#ifdef OS_LINUX

Poller::Poller() : fd_(epoll_create1(0)), events_(new epoll_event[MAX_EVENTS]) {
  assert(fd_ != -1);
  // ErrorIf(fd_ == -1, "epoll create error");

  memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Poller::~Poller() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
  delete[] events_;
}
// TODO(hua) day15的错误可能在这
std::vector<Channel *> Poller::Poll(int timeout) const {
  std::vector<Channel *> active_channels;
  int nfds = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
  if (nfds == -1) {
   perror("epoll wait error");
  }

  for (int i = 0; i < nfds; ++i) {
    Channel *channel_temp = (Channel *)events_[i].data.ptr;
    uint32_t events = events_[i].events;
    if (events & EPOLLIN) {
      channel_temp->SetReadyEvent(Channel::READ_EVENT);
    }
    if (events & EPOLLOUT) {
      channel_temp->SetReadyEvent(Channel::WRITE_EVENT);
    }
    if (events & EPOLLET) {
      channel_temp->SetReadyEvent(Channel::EDGE_TRIGGERED);
    }
    active_channels.push_back(channel_temp);
  }

  return active_channels;
}
// TODO(hua) day15的错误可能在这
RC Poller::UpdateChannel(Channel *_ch) const {
  int sockfd = _ch->Fd();
  struct epoll_event ev_temp {};
  ev_temp.data.ptr = _ch;
  if (_ch->ListenEvents() & Channel::READ_EVENT) {
    ev_temp.events |= EPOLLIN | EPOLLPRI;
  }
  if (_ch->ListenEvents() & Channel::WRITE_EVENT) {
    ev_temp.events |= EPOLLOUT;
  }
  if (_ch->ListenEvents() & Channel::EDGE_TRIGGERED) {
    ev_temp.events |= EPOLLET;
  }
  if (!_ch->Exist()) {
    if (epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev_temp) == -1) {
      perror("epoll add error");
      return RC_POLLER_ERROR;
    }
    // assert((epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev_temp) == -1) && "epoll add error");
  } else {
    if (epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev_temp) == -1) {
      perror("epoll modify error");
      return RC_POLLER_ERROR;
    }
    // assert((epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev_temp) == -1) && "epoll modify error");
  }
  return RC_SUCCESS;
}

RC Poller::DeleteChannel(Channel *_ch) const {
  int sockfd = _ch->Fd();
  if (epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1) {
    perror("epoll delete error");
    _ch->SetExist(false);
    return RC_POLLER_ERROR;
  }
  // assert((epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1) && "epoll delete error");
  // _ch->SetExist(false);
  return RC_SUCCESS;
}

#endif

#ifdef OS_MACOS

Poller::Poller() {
  fd_ = kqueue();
  ErrorIf(fd_ == -1, "kqueue create error");
  events_ = new struct kevent[MAX_EVENTS];
  memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Poller::~Poller() {
  if (fd_ != -1) {
    close(fd_);
  }
}

std::vector<Channel *> Poller::Poll(int timeout) {
  std::vector<Channel *> active_channels;
  struct timespec ts;
  memset(&ts, 0, sizeof(ts));
  if (timeout != -1) {
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000 * 1000;
  }
  int nfds = 0;
  if (timeout == -1) {
    nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, NULL);
  } else {
    nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, &ts);
  }
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].udata;
    int events = events_[i].filter;
    if (events == EVFILT_READ) {
      ch->SetReadyEvents(ch->READ_EVENT | ch->ET);
    }
    if (events == EVFILT_WRITE) {
      ch->SetReadyEvents(ch->WRITE_EVENT | ch->ET);
    }
    active_channels.push_back(ch);
  }
  return active_channels;
}

void Poller::UpdateChannel(Channel *ch) {
  struct kevent ev[2];
  memset(ev, 0, sizeof(*ev) * 2);
  int n = 0;
  int fd = ch->GetSocket()->GetFd();
  int op = EV_ADD;
  if (ch->GetListenEvents() & ch->ET) {
    op |= EV_CLEAR;
  }
  if (ch->GetListenEvents() & ch->READ_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_READ, op, 0, 0, ch);
  }
  if (ch->GetListenEvents() & ch->WRITE_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch);
  }
  int r = kevent(fd_, ev, n, NULL, 0, NULL);
  ErrorIf(r == -1, "kqueue add event error");
}

void Poller::DeleteChannel(Channel *ch) {
  struct kevent ev[2];
  int n = 0;
  int fd = ch->GetSocket()->GetFd();
  if (ch->GetListenEvents() & ch->READ_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch);
  }
  if (ch->GetListenEvents() & ch->WRITE_EVENT) {
    EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch);
  }
  int r = kevent(fd_, ev, n, NULL, 0, NULL);
  ErrorIf(r == -1, "kqueue delete event error");
}
#endif
