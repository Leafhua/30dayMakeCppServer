/**
 * @file Acceptor.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Acceptor.h"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <cstdio>
#include <functional>
#include <utility>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *_loop) : loop_(_loop), sock_(new Socket()), channel_(nullptr) {
  
  InetAddress *addr = new InetAddress("127.0.0.1", 8883);
  sock_->Bind(addr);
  sock_->Listen();
  //   sock->setnonblocking();
  channel_ = new Channel(loop_, sock_);
  auto cb = std::bind(&Acceptor::AcceptConnection, this);
  channel_->SetReadCallback(cb);
  channel_->EnableRead();
  delete addr;
}

Acceptor::~Acceptor() {
  delete sock_;
  delete channel_;
}

void Acceptor::AcceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
  clnt_sock->Setnonblocking();
  if (new_connection_callback_) {
  new_connection_callback_(clnt_sock);
  }
  
  delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> const &callback) { new_connection_callback_ = std::move(callback); }
