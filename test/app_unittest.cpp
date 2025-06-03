#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

extern "C" {
	#include "app.h"
}

class AppTest : public testing::Test {
	protected:
	app m_app;

	AppTest(){
		app_init(&m_app);
	}

	~AppTest() {
		app_terminate(&m_app);
	}
};

TEST_F(AppTest, initial_state){
	ASSERT_EQ(m_app.n_servers, 0);
	ASSERT_EQ(m_app.servers, NULL);
	ASSERT_NE(m_app.epoll, 0) << "no epoll fd was created";
}

TEST_F(AppTest, attach_server){
	
	int opt = 1;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	bind(listenfd, (sockaddr*)&addr, sizeof(addr));

	server_cfg cfg = {
		.listenfd = listenfd
	};

	ASSERT_EQ(app_add_server(&m_app, &cfg), 0) << "error attaching server to app";
}