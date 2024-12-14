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
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <cstdio>
#include <functional>
#include <netinet/in.h>

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop),sock(nullptr), accepChannel(nullptr) {
  sock = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8883);
  sock->bind(addr);
  sock->listen();
//   sock->setnonblocking();
  accepChannel = new Channel(loop, sock->getFd());
  auto cb = std::bind(&Acceptor::acceptConnection, this);
  accepChannel->setReadCallback(cb);
  accepChannel->enableRead();
  accepChannel->setUseThreadPool(false);
  delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete accepChannel;

}

void Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP:%s Port:%d\n",clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void (Socket *)> _cb) {
    newConnectionCallback = _cb;
}
