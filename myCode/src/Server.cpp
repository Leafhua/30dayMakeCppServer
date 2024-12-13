/**
 * @file Server.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Server.h"
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {

  acceptor = new Acceptor(loop);
  auto cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor->setNewConnectionCallback(cb);


}

Server::~Server() {
  delete acceptor;
}

void Server::handleReadEvent(int sockfd) {
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

void Server::newConnection(Socket *serv_sock) {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
         inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
  clnt_sock->setnonblocking();
  Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
  //测试auto
  auto cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
  clntChannel->setCallback(cb);
  clntChannel->enableReading();
}
