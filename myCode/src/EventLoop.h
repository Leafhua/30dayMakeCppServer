/**
 * @file EventLoop.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once


#include <functional>
class ThreadPool;
class Epoll;
class Channel;
class EventLoop {
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);

    void addThread(std::function<void()>);

};

