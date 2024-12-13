/**
 * @file Connection.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <functional>
class EventLoop;
class Channel;
class Socket;
class Connection {

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket*)> deleteConnectionCallback;

public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void (Socket*)>);
    
};


