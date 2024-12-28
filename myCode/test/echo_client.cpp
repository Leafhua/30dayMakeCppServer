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

#include <iostream>
#include <string>
#include "pine.h"
int main() {
  Socket *sock = new Socket();
  sock->Create();
  sock->Connect("127.0.0.1", 8883);

  Connection *conn = new Connection(sock->Fd(), nullptr);

  while (true) {
    std::string input;
    std::getline(std::cin, input);
    conn->SetSendBuf(input.c_str());
    conn->Write();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      break;
    }
    conn->Read();
    std::cout << "Message from server : " << conn->ReadBuf()->CStr() << std::endl;
  }
  delete conn;
  delete sock;
  return 0;
}
