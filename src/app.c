#include "app.h"

#include <sys/epoll.h>
#include <unistd.h>

int app_init(struct app* result){
	if(!result) return -1;

	*result = (struct app){0};

	result->epoll = epoll_create1(EPOLL_CLOEXEC);

	return 0;
}

int app_add_srv(struct server* srv) {
	return -1;
}

int app_terminate(struct app* app){
	close(app->epoll);

	*app = (struct app) {0};
	return 0;
}
