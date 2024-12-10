CURRENT_DIR = $(shell pwd)/myCode/

server:
	g++ $(CURRENT_DIR)server.cpp $(CURRENT_DIR)util.cpp $(CURRENT_DIR)InetAddress.cpp $(CURRENT_DIR)Socket.cpp $(CURRENT_DIR)Epoll.cpp -o $(CURRENT_DIR)server

client:
	g++ $(CURRENT_DIR)client.cpp $(CURRENT_DIR)util.cpp -o $(CURRENT_DIR)client
# 定义伪目标
.PHONY: clean
clean:
	rm  server client