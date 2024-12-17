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
#include "Connection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <netinet/in.h>
#include <strings.h>
#include <thread>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : mainReadctor(_loop), acceptor(nullptr) {

  acceptor = new Acceptor(mainReadctor);
  auto cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor->setNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();
  thpool = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    subReadctors.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop =
        std::bind(&EventLoop::loop, subReadctors[i]);
    thpool->add(sub_loop);
  }
}

Server::~Server() {
  delete acceptor;
  delete thpool;
}

void Server::newConnection(Socket *sock) {

  if (sock->getFd() != -1) {
    int random = sock->getFd() %subReadctors.size();
    Connection *conn = new Connection(subReadctors[random], sock);
    auto cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
  }
}

void Server::deleteConnection(int sockfd) {
  if (sockfd != -1) {
    auto it = connections.find(sockfd);
    if (it != connections.end()) {
      Connection *conn = connections[sockfd];
      connections.erase(sockfd);
      //close(sockfd);
      delete conn;
    }
  }
}
