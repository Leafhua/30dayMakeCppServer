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
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
private:
    EventLoop *mainReadctor;
    Acceptor *acceptor;
    ThreadPool *thpool;

    std::map<int, Connection*> connections;
    std::vector<EventLoop *> subReadctors;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(int sockfd);
    
};