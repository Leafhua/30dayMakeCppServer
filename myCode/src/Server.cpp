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



void Server::newConnection(Socket *sock) {

  Connection *conn = new Connection(loop, sock);
  auto cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
  conn->setDeleteConnectionCallback(cb);
  connections[sock->getFd()] = conn;

}

void Server::deleteConnection(Socket *sock) {
  Connection *conn = connections[sock->getFd()];
  connections.erase(sock->getFd());
  delete conn;
}
