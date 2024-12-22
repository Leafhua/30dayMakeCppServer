/**
 * @file echo_server.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstdlib>
#include <iostream>
#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include "SignalHandler.h"
#include "Socket.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);

  Signal::signal(SIGINT, [&] {
    delete server;
    delete loop;
    std::cout << "\nServer exit!" << std::endl;
    exit(0);
  });

  server->NewConnect(
      [](Connection *conn) { std::cout << "New Connection fd: " << conn->GetSocket()->GetFd() << std::endl; });
  server->OnMessage([](Connection *conn) {
    std::cout << "Message from client " << conn->ReadBuffer() << std::endl;
    if (conn->GetState() == Connection::State::Connected) {
      conn->Send(conn->ReadBuffer());
    }
  });
  loop->Loop();
  return 0;
}
