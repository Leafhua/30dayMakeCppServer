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
#include <string>
class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection {

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    Buffer *readBuffer;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;


public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)>);
    void send(int sockfd);
    
};


