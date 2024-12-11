/**
 * @file server.__cpp_guaranteed_copy_elision
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Channel.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
};
void handleReadEvent(int sockfd) {
  char buf[READ_BUFFER];
  while (true) {
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      printf("message from client fd %d: %s\n", sockfd, buf);
      write(sockfd, buf, sizeof(buf));

    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading\n");
      continue;

    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
      printf("finish reading onece, errno: %d\n", errno);
      break;
    } else if (bytes_read == 0) {
      printf("EOF, client fd %d disconnected!\n", sockfd);
      close(sockfd);
      break;
    }
  }
}

int main() {
  Socket *serv_sock = new Socket();
  InetAddress *serv_addr = new InetAddress("127.0.0.1", 8883);
  serv_sock->bind(serv_addr);
  serv_sock->listen();
  Epoll *ep = new Epoll();
  serv_sock->setnonblocking();
  Channel *serv_channel = new Channel(ep, serv_sock->getFd());
  serv_channel->enableReading();
  // 使用上述代码设置ET
  // ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
  while (true) {
    std::vector<Channel *> activeChannels = ep->poll();
    int nfds = activeChannels.size();
    for (int i = 0; i < nfds; ++i) {
      int chfd = activeChannels[i]->getFd();
      if (chfd == serv_sock->getFd()) {
        InetAddress *clnt_addr = new InetAddress();
        Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
        printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
               inet_ntoa(clnt_addr->addr.sin_addr),
               ntohs(clnt_addr->addr.sin_port));
        clnt_sock->setnonblocking();
        Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
        clntChannel->enableReading();
        // 使用上述代码设置ET
        // ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);

      } else if (activeChannels[i]->getEvents() & EPOLLIN) {
        handleReadEvent(activeChannels[i]->getFd());

      } else {
        printf("something else happened\n");
      }
    }
  }

  delete serv_sock;
  delete serv_addr;
  return 0;
}
