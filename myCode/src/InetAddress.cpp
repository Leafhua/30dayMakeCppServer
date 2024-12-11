/**
 * @file InetAddress.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <strings.h>

InetAddress::InetAddress() : addr_len(sizeof(addr)) {
  bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char *ip, uint16_t port)
    : addr_len(sizeof(addr)) {
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  addr_len = sizeof(addr);
}

InetAddress::~InetAddress() {}