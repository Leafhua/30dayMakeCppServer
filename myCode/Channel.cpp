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
#include <cstdint>
#include <sys/epoll.h>

Channel::Channel(Epoll *_ep, int _fd)
    : ep(_ep), fd(_fd), events(0), revents(0), inEpoll(false) {}
Channel::~Channel() {}
void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }

uint32_t Channel::getRevents() { return revents; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll() { inEpoll = true; }

void Channel::setRevents(uint32_t _ev) {
    revents = _ev;
}
