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
#include "common.h"
#include "pine.h"

int main() {
  TcpServer *server = new TcpServer();

  Signal::signal(SIGINT, [&] {
    delete server;
    std::cout << "\nServer exit!" << std::endl;
    exit(0);
  });

  server->OnConnect(
      [](Connection *conn) { std::cout << "New Connection fd: " << conn->GetSocket()->Fd() << std::endl; });

  server->OnRecv([](Connection *conn) {
    std::cout << "Message from client " << conn->ReadBuf()->CStr() << std::endl;
    conn->Send(conn->ReadBuf()->CStr());
  });

  server->Start();

  delete server;
  return 0;
}
