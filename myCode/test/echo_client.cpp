/**
 * @file single_client.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
int main() {
  Socket *sock = new Socket();
  sock->Connect("127.0.0.1", 8883);

  Connection *conn = new Connection(nullptr, sock);

  while (true) {
    conn->GetlineSendBuffer();
    conn->Write();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      break;
    }
    conn->Read();
    std::cout << "Message from server : " << conn->ReadBuffer() << std::endl;
  }
  delete conn;
  return 0;
}
