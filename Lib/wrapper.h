#ifndef __WRAPPER_H_
#define __WRAPPER_H_

#include "error.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>

#define max(a,b) (a>b? a:b)
// wrap socket
int _Accept(int fd, struct sockaddr *sa, socklen_t *sa_length_ptr);
int _Socket(int family, int type, int protocol);
void _Bind(int fd, const struct sockaddr *sa, socklen_t sa_length);
void _Connect(int fd, const struct sockaddr *sa, socklen_t sa_length);
void _Listen(int fd, int backlog);
int _Select(int nFds, fd_set *readFds, fd_set *writeFds, fd_set *exceptFds, struct timeval *timeout);
void _Shutdown(int fd, int how);

void _Write(int fd, void *ptr, size_t nbytes);

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
#endif