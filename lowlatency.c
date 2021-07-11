#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>

int (*real_bind)(int, const struct sockaddr *, socklen_t);
int (*real_connect)(int, const struct sockaddr *, socklen_t);
int (*real_read)(int, void *, size_t);

char *bind_addr_env;
unsigned long int bind_addr_saddr;
unsigned long int inaddr_any_saddr;

void _init(void)
{
    const char *err;

    real_bind = dlsym(RTLD_NEXT, "bind");
    if ((err = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym (bind): %s\n", err);
    }

    real_connect = dlsym(RTLD_NEXT, "connect");
    if ((err = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym (connect): %s\n", err);
    }

    real_read = dlsym(RTLD_NEXT, "read");
    if ((err = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym (read): %s\n", err);
    }
}

int bind(int fd, const struct sockaddr *sk, socklen_t sl)
{
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&one, sizeof(one));
    setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (void *)&one, sizeof(one));
    
    return real_bind(fd, sk, sl);
}

int connect(int fd, const struct sockaddr *sk, socklen_t sl)
{
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&one, sizeof(one));
    setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (void *)&one, sizeof(one));

    return real_connect(fd, sk, sl);
}

int read(int fd, void *buf, size_t count)
{
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (void *)&one, sizeof(one));

    return real_read(fd, buf, count);
}