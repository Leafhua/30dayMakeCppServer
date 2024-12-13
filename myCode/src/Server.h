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

#include <map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server {
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    
};