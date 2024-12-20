/**
 * @file client.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-07
 *
 *
 */
#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "util.h"

#define BUFFER_SIZE 1024

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ErrorIf(sockfd == -1, "socket create error");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8883);
  ErrorIf(connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

  while (true) {
    char buf[BUFFER_SIZE];
    bzero(&buf, sizeof(buf));
    scanf("%s", buf);
    ssize_t write_bytes = write(sockfd, buf, sizeof(buf));

    if (write_bytes == -1) {
      printf("socket write error, can't write any more!\n");
      break;
    }
    bzero(&buf, sizeof(buf));
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    if (read_bytes > 0) {
      printf("message from server: %s\n", buf);
    } else if (read_bytes == 0) {
      printf("server socket disconnected!\n");
      break;
    } else if (read_bytes == -1) {
      close(sockfd);
      ErrorIf(true, "socket read error\n");
    }
  }
  close(sockfd);
  return 0;
}
