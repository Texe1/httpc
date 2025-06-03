#include "app.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <stdlib.h>

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

	if(app->servers){
		for(int i = 0; i < app->n_servers; i++){
			server_terminate(&(app->servers[i]));
		}
	}
	free(app->servers);

	*app = (struct app) {0};
	return 0;
}

int app_add_server(struct app* app, struct server_cfg* cfg){
	if(!app || !cfg) return -1;

	void* tmp;
	if(app->servers == NULL || app->n_servers == 0) {
		app->n_servers = 0;
		tmp = malloc(sizeof(struct server));
	}else {
		tmp = realloc(app->servers, sizeof(struct server) * (app->n_servers + 1));
	}

	if(tmp == NULL) {
		return 1;
	}

	app->servers = tmp;

	server_init(&app->servers[app->n_servers], cfg);
	app->n_servers++;

	return 0;
}