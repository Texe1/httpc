#include "server.h"

#include "sys/epoll.h"

void* server_thread_main(void*);

int create_server(struct server* srv, struct server_cfg* cfg){
	if(srv == NULL) return -1;
	srv->listenfd = cfg->listenfd;
	srv->epoll = epoll_create1(EPOLL_CLOEXEC);

	return pthread_create(&(srv->main_thread), NULL, server_thread_main, (void*) srv);
}

void* server_thread_main(void* data){
	struct server* srv = (struct server*) data;

	return data;

	// TODO accept() loop
}