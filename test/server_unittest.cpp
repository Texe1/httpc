#include <gtest/gtest.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

extern "C" {
	#include "server.h"
}

class ServerTest : public testing::Test {
protected:
	server m_srv;
	server_cfg m_cfg = { 0 };

	ServerTest() {
		int opt = 1;
		m_cfg.listenfd = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(m_cfg.listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(8080);

		bind(m_cfg.listenfd, (sockaddr*)&addr, sizeof(addr));
		server_init(&m_srv, &m_cfg);
	}

	~ServerTest() {
		server_terminate(&m_srv);
	}
};


TEST_F(ServerTest, creation){
	EXPECT_NE(m_srv.listenfd, 0);
	EXPECT_NE(m_srv.epoll, 0);
	EXPECT_NE(m_srv.main_thread, 0);

	EXPECT_EQ(m_srv.active, 1);
}

TEST_F(ServerTest, response){
    const char* hello = "Hello from client";
    char buffer[1024] = { 0 };

	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_GE(client_fd, 0) << "Socket creation error (" << errno << ")";

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	struct timeval timeout = {
		.tv_sec = 1,
		.tv_usec = 0
	};
	setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (void*) &timeout, sizeof(timeout));


    // Convert IPv4 and IPv6 addresses from text to binary
    // form
	printf("0\n");

	bind(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	ASSERT_EQ(connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)), 0) << "Connection Failed (" << errno << ")";
  
	printf("1\n");

    send(client_fd, hello, strlen(hello), 0);
	usleep(200);
	printf("2\n");
    EXPECT_GE(read(client_fd, buffer,1024 - 1), 0); 
	printf("3\n");
    printf("%s\n", buffer);

	EXPECT_STREQ(buffer, "HTTP/1.1 200 OK\r\n\r\n<h1>Hello</h1>");

    // closing the connected socket
    EXPECT_EQ(close(client_fd), 0);
}