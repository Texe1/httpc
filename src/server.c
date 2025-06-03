#include "server.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include <errno.h>

#include <sys/time.h>

#include <stdio.h>

void* server_thread_main(void*);

int server_init(struct server* srv, struct server_cfg* cfg){
	if(srv == NULL) return -1;
	if(cfg->listenfd == 0) return 1;
	srv->listenfd = cfg->listenfd;
	srv->epoll = epoll_create1(EPOLL_CLOEXEC);
	if(srv->epoll == 0) return 2;
	srv->active = 1;

	struct timeval timeout = {
		.tv_sec = 1,
		.tv_usec = 0
	};
	setsockopt(srv->listenfd, SOL_SOCKET, SO_RCVTIMEO, (void*) &timeout, sizeof(timeout));

	return pthread_create(&(srv->main_thread), NULL, server_thread_main, (void*) srv);
}

int server_terminate(struct server* srv){
	srv->active = 0;
	if(srv->main_thread){
		pthread_join(srv->main_thread, NULL);
	}
	close(srv->listenfd);
	close(srv->epoll);
}

void* server_thread_main(void* data){
	struct server* srv = (struct server*) data;

	int clientfd = 0;
	struct sockaddr addr;
	socklen_t addr_len;

	listen(srv->listenfd, 100);

	char in_buf[1024] = {0};
	char msg[] = "HTTP/1.1 200 OK\r\n\r\n<h1>Hello</h1>";
	
	while(srv->active){
		clientfd = accept(srv->listenfd, &addr, &addr_len);
		if(clientfd == -1) {
			continue;
		}
		int valread = recv(clientfd, in_buf, 1023, 0);
		printf("%s\n", in_buf);
		send(clientfd, msg, sizeof(msg), 0);
		close(clientfd);
	}
	srv->main_thread = 0;
	return NULL;
}