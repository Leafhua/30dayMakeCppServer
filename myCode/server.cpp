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
 
#include "util.h"
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
};

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv_addr;

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8883);
  errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1,
        "socket bind error");
  errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

  int epfd = epoll_create1(0);
  struct epoll_event events[MAX_EVENTS], ev;
  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

  while (true) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == sockfd) {

        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len = sizeof(clnt_addr);
        bzero(&clnt_addr, sizeof(clnt_addr));
        int clnt_sockfd =
            accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
        errif(clnt_sockfd == -1, "socket accept error");
        printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd,
               inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        bzero(&ev, sizeof(ev));
        ev.data.fd = clnt_sockfd;
        ev.events = EPOLLIN;
        setnonblocking(clnt_sockfd);
        epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
      } else if (events[i].events & EPOLLIN) {
        char buf[READ_BUFFER];
        while (true) {
          bzero(&buf, sizeof(buf));
          ssize_t read_bytes = read(events[i].data.fd, buf, sizeof(buf));

          if (read_bytes > 0) {
            printf("message from client fd %d: %s\n", events[i].data.fd, buf);
            write(events[i].data.fd, buf, sizeof(buf));

          } else if (read_bytes == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
          } else if (read_bytes == -1 &&
                     ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading onece ,errno: %d\n", errno);
            break;
          } else if (read_bytes == 0) {
            printf("EOF, client fd %d disconnected!\n", events[i].data.fd);
            close(events[i].data.fd);
            break;
          }
        }
      } else {
        printf("something else happened\n");
      }
    }
  }
  close(sockfd);
  return 0;
}
