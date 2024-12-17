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

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *_loop) : loop_(_loop), sock_(nullptr), channel_(nullptr) {
  sock_ = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8883);
  sock_->Bind(addr);
  sock_->Listen();
  //   sock->setnonblocking();
  channel_ = new Channel(loop_, sock_->GetFd());
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
  printf("new client fd %d! IP:%s Port:%d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr),
         ntohs(clnt_addr->GetAddr().sin_port));
  clnt_sock->Setnonblocking();
  new_connection_callback_(clnt_sock);
  delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> _cb) { new_connection_callback_ = _cb; }
