/**
 * @file InetAddress.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

//包括了 <netinet/in.h>，<netinet/in.h>中其实也包括了<sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// #include <netinet/in.h>
class InetAddress {
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
};