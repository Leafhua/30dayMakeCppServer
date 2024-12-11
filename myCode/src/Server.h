/**
 * @file Server.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "EventLoop.h"
#include "Socket.h"
class EventLoop;
class Socket;
class Server {
private:
    EventLoop *loop;

public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    
};