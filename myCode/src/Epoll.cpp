#include "Epoll.h"

#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <vector>

#include "Channel.h"
#include "util.h"

#define MAX_EVENTS 1024

Epoll::Epoll() : epfd_(-1), events_(nullptr) {
  epfd_ = epoll_create1(0);
  ErrorIf(epfd_ == -1, "epoll create error");
  events_ = new epoll_event[MAX_EVENTS];
  bzero(events_, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }
  delete[] events_;
}

// 新的 poll实现
std::vector<Channel *> Epoll::Poll(int timeout) {
  std::vector<Channel *> activeChannels;
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
  ErrorIf(nfds == -1, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].data.ptr;
    ch->SetReadyEvents(events_[i].events);
    activeChannels.push_back(ch);
  }
  return activeChannels;
}

void Epoll::UpdateChannel(Channel *channel) {
  int fd = channel->GetFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = channel;
  ev.events = channel->GetListenEvents();
  if (!channel->GetInEpoll()) {
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    channel->SetInEpoll();
  } else {
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}
