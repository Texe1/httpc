#pragma once

#include "pthread.h"

struct server {
	int listenfd;
	pthread_t main_thread;

	int epoll;
	// TODO state change queue
	// TODO list of instances
	// TODO protocol state
	// TODO connection queue
};

struct server_cfg {
	int listenfd;
};


int create_server(struct server*, struct server_cfg*);