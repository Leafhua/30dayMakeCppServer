/**
 * @file Acceptor.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <functional>
class EventLoop;
class Socket;
class Channel;
class Acceptor {

private:
    EventLoop *loop;
    Socket *sock;
    Channel *accepChannel;
    std::function<void(Socket*)> newConnectionCallback;

public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();


    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);

};