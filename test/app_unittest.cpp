#include <gtest/gtest.h>

extern "C" {
	#include "app.h"
}

TEST(app_init, epoll_creation){
	app a;
	ASSERT_EQ(app_init(&a),0) << "app_init returned with error";
	EXPECT_NE(a.epoll, 0) << "no epoll fd was created";
}