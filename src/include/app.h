#pragma once

#include "server.h"

/*
The App struct stores application level information that is shared between servers.
This may include user details, 
*/ 
struct app {
	int epoll;

	int n_servers;
	struct server* servers;
	// TODO list of attached servers
	// TODO command queue
	// TODO application state
};

int app_init(struct app*);
int app_terminate(struct app*);

int app_add_server(struct app*, struct server_cfg*);