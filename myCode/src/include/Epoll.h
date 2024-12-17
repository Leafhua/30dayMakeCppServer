#pragma once

#include <sys/epoll.h>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Macros.h"
class Channel;
class Epoll {
 public:
  Epoll();
  ~Epoll();
  DISALLOW_COPY_AND_MOVE(Epoll);
  // void addFd(int fd, uint32_t op);
  void UpdateChannel(Channel *);
  void DeleteChannel(Channel *);
  std::vector<Channel *> Poll(int timeout = -1);

 private:
  int epfd_;
  struct epoll_event *events_;
};
