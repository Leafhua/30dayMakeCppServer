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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <utility>
#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : main_readctor_(_loop), acceptor_(nullptr) {
  acceptor_ = new Acceptor(main_readctor_);
  auto cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();
  thread_pool_ = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    sub_readctors_.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_readctors_[i]);
    thread_pool_->Add(sub_loop);
  }
}

Server::~Server() {
  delete acceptor_;
  delete thread_pool_;
}

void Server::NewConnection(Socket *sock) {
  ErrorIf(sock->GetFd() == -1, "new connection error");
  uint64_t random = sock->GetFd() % sub_readctors_.size();
  Connection *conn = new Connection(sub_readctors_[random], sock);
  auto cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  conn->SetDeleteConnectionCallback(cb);
  conn->SetOnConnectCallback(on_connect_callback_);
  connections_[sock->GetFd()] = conn;
}

void Server::DeleteConnection(Socket *sock) {
  int sockfd = sock->GetFd();
  auto it = connections_.find(sockfd);
  if (it != connections_.end()) {
    Connection *conn = connections_[sockfd];
    connections_.erase(sockfd);
    // close(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { on_connect_callback_ = std::move(fn); }
