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
#include "Exception.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : main_readctor_(_loop), acceptor_(nullptr), thread_pool_(nullptr) {
  
  if (main_readctor_ == nullptr) {
    throw Exception(ExceptionType::INVALID, "main reactor can't be nullptr");
  }

  acceptor_ = new Acceptor(main_readctor_);
  auto cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = static_cast<int>(std::thread::hardware_concurrency());
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
  for (EventLoop *each: sub_readctors_) {
    delete each;
  }
  delete acceptor_;
  delete thread_pool_;
}

void Server::NewConnection(Socket *sock) {
  if (sock->GetFd() == -1) {
    throw Exception(ExceptionType::INVALID_SOCKET, "New Connection error, invalid client socket!");
  }
  // ErrorIf(sock->GetFd() == -1, "new connection error");

  uint64_t random = sock->GetFd() % sub_readctors_.size();
  Connection *conn = new Connection(sub_readctors_[random], sock);
  auto cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  conn->SetDeleteConnectionCallback(cb);
  // conn->SetOnConnectCallback(on_connect_callback_);
  conn->SetOnMessageCallback(on_message_callback_);
  connections_[sock->GetFd()] = conn;
  if (new_connection_callback_) {
    new_connection_callback_(conn);
  }
}

void Server::DeleteConnection(Socket *sock) {
  int sockfd = sock->GetFd();
  auto it = connections_.find(sockfd);
  if (it != connections_.end()) {
    Connection *conn = connections_[sockfd];
    connections_.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> _fn) { on_connect_callback_ = std::move(_fn); }

void Server::OnMessage(std::function<void (Connection *)> _fn) { on_message_callback_ = std::move(_fn); }

void Server::NewConnect(std::function<void (Connection *)> _fn) { new_connection_callback_ = std::move(_fn); }
