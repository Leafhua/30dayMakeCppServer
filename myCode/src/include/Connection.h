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
#include "Macros.h"
class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection {
 public:
 enum State {
  Invalid = 1,
  Hadnshaking,
  Connected,
  Closed,
  Failed,

 };
  Connection(EventLoop *_loop, Socket *_sock);
  ~Connection();
  DISALLOW_COPY_AND_MOVE(Connection)

  void Read();
  void Write();

  void SetOnConnectCallback(std::function<void(Connection *)> const &callback);
  void SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
  void Close();
  State GetState();
  
  Buffer *GetReadBuffer();
  const char *ReadBuffer();

  void SetSendBuffer(const char *str);
  Buffer *GetSendBuffer();
  const char *SendBuffer();
  void GetlineSendBuffer();

  Socket *GetSocket();

  void OnConnect(std::function<void()> fn);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_{nullptr};
  State state_{State::Invalid};
  Buffer *read_buffer_{nullptr};
  Buffer *send_buffer_{nullptr};
  std::function<void(Socket *)> delete_connectioin_callback_;

  std::function<void(Connection *)> on_connect_callback_;

  void ReadNonBlocking();
  void WriteNonBlocking();
  void ReadBlocking();
  void WriteBlocking();
};
