/**
 * @file Channel.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd)
    : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false) {}
Channel::~Channel() {
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent() {
    callback();
}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }

uint32_t Channel::getRevents() { return revents; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll() { inEpoll = true; }

void Channel::setRevents(uint32_t _ev) {
    revents = _ev;
}

void Channel::setCallback(std::function<void ()> _cb) {
    callback = _cb;
}
