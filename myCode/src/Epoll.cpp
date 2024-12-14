#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1024

Epoll::Epoll() : epfd(-1), events(nullptr) {
  epfd = epoll_create1(0);
  errif(epfd == -1, "epoll create error");
  events = new epoll_event[MAX_EVENTS];
  bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (epfd != -1) {
    close(epfd);
    epfd = -1;
  }
  delete[] events;
}

//新的 poll实现
std::vector<Channel *> Epoll::poll(int timeout) {
  std::vector<Channel *> activeChannels;
  int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
  errif(nfds == -1, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel*)events[i].data.ptr;
    ch->setReady(events[i].events);
    activeChannels.push_back(ch);
  }
  return activeChannels;
}

void Epoll::updateChannel(Channel *channel) {
  int fd = channel->getFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = channel;
  ev.events = channel->getEvents();
  if (!channel->getInEpoll()) {
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    channel->setInEpoll();
  } else {
    errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}
