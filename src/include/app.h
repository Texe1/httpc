#pragma once

#include "server.h"

struct app {
	int epoll;

	int n_servers;
	struct server* servers;
	// TODO list of attached servers
	// TODO command queue
	// TODO application state
};

int app_init(struct app* result);
int app_terminate(struct app* app);