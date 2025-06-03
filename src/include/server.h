#pragma once

#include "pthread.h"
struct server {
	int listenfd;
	pthread_t main_thread;
	unsigned int active :1;
	int epoll;
	// TODO state change queue
	// TODO list of instances
	// TODO protocol state
	// TODO connection queue
};

struct server_cfg {
	int listenfd;
};


int server_init(struct server*, struct server_cfg*);
int server_terminate(struct server*);