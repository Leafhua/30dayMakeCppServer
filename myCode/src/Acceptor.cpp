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
#include <functional>

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop) {
  sock = new Socket();
  addr = new InetAddress("127.0.0.1", 8883);
  sock->bind(addr);
  sock->listen();
  sock->setnonblocking();
  accepChannel = new Channel(loop, sock->getFd());
  auto cb = std::bind(&Acceptor::acceptConnection, this);
  accepChannel->setCallback(cb);
  accepChannel->enableReading();
}

Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete accepChannel;

}

void Acceptor::acceptConnection() {
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void (Socket *)> _cb) {
    newConnectionCallback = _cb;
}
