#include <gtest/gtest.h>

extern "C" {
	#include "server.h"
}

TEST(create_server, thread_creation){
	struct server srv;
	struct server_cfg cfg = { 0 };
	ASSERT_EQ(create_server(&srv, &cfg), 0);

	void* res = NULL;

	pthread_join(srv.main_thread, &res);
	EXPECT_EQ(res, (void*)&srv);
}