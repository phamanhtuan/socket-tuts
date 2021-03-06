#ifndef __WRAPPER_H_
#define __WRAPPER_H_

#include "error.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <pthread.h>
#include <netdb.h>

#define max(a,b) (a>b? a:b)
#define min(a,b) (a<b? a:b)

// wrap socket
int _Accept(int fd, struct sockaddr *sa, socklen_t *sa_length_ptr);
int _Socket(int family, int type, int protocol);
void _Bind(int fd, const struct sockaddr *sa, socklen_t sa_length);
void _Connect(int fd, const struct sockaddr *sa, socklen_t sa_length);
void _Listen(int fd, int backlog);
int _Select(int nFds, fd_set *readFds, fd_set *writeFds, fd_set *exceptFds, struct timeval *timeout);
void _Shutdown(int fd, int how);
void _Getsockname(int fd, struct sockaddr *sa, socklen_t sa_len_ptr);
void _Socketpair(int family, int type, int protocol, int *fd);
// void _Getsockopt(int fd, int level, int optname, void *opt_val, socklen_t *opt_len_ptr);
// void _Setsockopt(int fd, int level, int opt_name, const void *opt_val, socklen_t optlen);
void _Write(int fd, void *ptr, size_t nbytes);
void _Getsockopt(int fd, int level, int optname, void *optval, socklen_t *opt_len_ptr);
void _Setsockopt(int fd, int level, int optname, const void *optval, socklen_t opt_len);
pid_t _Fork();

void myLog(const char *msg);

int localSockfdPrint(int _sockFd);
int	foreignSockfdPrint(int _sockFd);

// wrap stdio
char* _Fgets(char *ptr, int n, FILE *stream);
void _Fputs(char *ptr, FILE *stream);

//readn, writen
ssize_t _read_n(int fd, void *vptr, size_t n);
ssize_t _Read_n(int fd, void *ptr, size_t nbytes);
ssize_t _read_line(int fd, void *vptr, size_t max_len);
ssize_t _Read_line(int fd, void *ptr, size_t max_len);

ssize_t _write_n(int fd, const void *vptr, size_t n);
void _Write_n(int fd, void *ptr, size_t nbytes);

// Unix
ssize_t _Read(int fd, void *ptr, size_t nbytes);
pid_t _Wait_pid(pid_t pid, int *iptr, int options);
int _Fcntl(int fd, int cmd, int arg);
// Thread
void _Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(func)(void *), void *arg);
void _Pthread_detach(pthread_t tid);
void *_Malloc(size_t size);
void _Pthread_mutext_lock(pthread_mutex_t *mptr);
void _Pthread_mutext_unlock(pthread_mutex_t *mptr);
void _Pthread_cond_signal(pthread_cond_t *cptr);
void _Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
void _Pthread_join(pthread_t tid, void **status);

// Tcp_connect
int tcp_connect(const char *host, const char *serv);
struct addrinfo* _Host_serv(const char *host, const char *serv, int family, int sock_type);

#endif