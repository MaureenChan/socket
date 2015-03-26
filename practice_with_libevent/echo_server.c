#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#define LISTER_PORT 9999
#define LISTEN_BACKLOG 32
void do_accept(evutil_socket_t listener, short event, void *arg);
void read_cb(struct bufferevent *bev, void *arg);
void error_cb(struct bufferevent *bev, short event, void *arg);
void write_cb(struct bufferevent *bev, void *arg);
int main(int argc, char *argv[]) {
    int ret;
    evutil_socket_t listener;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listerner > 0);
    evutil_make_listen_
}
