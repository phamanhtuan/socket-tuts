#include "wrapper.h"

int _Accept(int fd, struct sockaddr *sa, socklen_t *sa_length_ptr){
	int n;
	n = accept(fd, sa, sa_length_ptr);
	if(n<0)
		error("ERROR: accept error");
	return n;

}
int _Socket(int family, int type, int protocol){
	int n;
	n = socket(family, type, protocol);
	if(n<0)
		error("ERROR: socket error");
	return n;
}
void _Bind(int fd, const struct sockaddr *sa, socklen_t sa_length){
	if(bind(fd, sa, sa_length) < 0)
		error("ERROR: bind error");
}
void _Connect(int fd, const struct sockaddr *sa, socklen_t sa_length){
	if(connect(fd, sa, sa_length) <0)
		error("ERROR: connect error");
}
void _Listen(int fd, int backlog){
	if(listen(fd, backlog) < 0)
		error("ERROR: listen error");
}


void _Write(int fd, void *ptr, size_t nbytes){
	if(write(fd, ptr, nbytes) != nbytes)
		error("ERROR: write error");
}

pid_t _Fork(){
	pid_t pid;
	pid = fork();	
	if(pid == -1)
		error("ERROR: fork error");
	return pid;
}

void myLog(const char *msg){
	printf("%s\n", msg );
}

int foreignSockfdPrint(int _sockFd){
	struct sockaddr_in ss;
	socklen_t len;
	len = sizeof(ss);
	int port = getpeername(_sockFd, (struct sockaddr *) &ss, &len);
	if(port < 0)
		return -1;
	printf("Foreign IP: %s\n", inet_ntoa(ss.sin_addr));
	printf("Foreign Port: %d\n", (ss.sin_port));
	return 0;
}
int localSockfdPrint(int _sockFd){
	struct sockaddr_storage ss;
	socklen_t len;
	len = sizeof(ss);
	int port = -1;
	char ipstr[INET6_ADDRSTRLEN];
	int result = getsockname(_sockFd, (struct sockaddr *) &ss, &len);
	if(result < 0)
		return -1;

	if(ss.ss_family == AF_INET){
		struct sockaddr_in *s = (struct sockaddr_in *) &ss;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
	}
	printf("Local IP: %s\n", ipstr);
	printf("Local Port: %d\n", port);
	return 0;
}


char* _Fgets(char *ptr, int n, FILE *stream){
	char *rptr;
	rptr = fgets(ptr, n, stream);
	if(rptr == NULL && ferror(stream))
		error("ERROR(_Fgets): fgets error");
	return rptr;
}	
void _Fputs(char *ptr, FILE *stream){
	if(fputs(ptr, stream) == EOF)
		error("ERROR: fputs error");
}
ssize_t _read_n(int fd, void *vptr, size_t n){
	printf("_read_n function: n=%d\n", n);
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;	
	while(nleft > 0){
		nread=read(fd, ptr, nleft);
		printf("_read_n function: nread=%d\n", nread);
		if(nread <0){
			if(errno == EINTR)
				nread =0;
			else
				return -1;			
		}else
			if(nread == 0)
				break;
		nleft -=nread;
		ptr+=nread;
	}
	return (n-nleft);
}
ssize_t _Read_n(int fd, void *ptr, size_t nbytes){
	ssize_t n;
	if((n=_read_n(fd, ptr, nbytes))<0)
		error("ERROR(_Readn): readn error");
	return n;
}

ssize_t _write_n(int fd, const void *vptr, size_t n){
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;

	while(nleft > 0){
		nwritten = write(fd, ptr, nleft);
		if(nwritten <= 0 )
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		nleft -=nwritten;
		ptr+=nwritten;	
	}
	return n;
}
void _Write_n(int fd, void *ptr, size_t nbytes){
	if(_write_n(fd, ptr, nbytes) != nbytes)
		error("ERROR(_Write_n): written error");
}
static int read_cnt;
static char *read_ptr;
static char read_buf[4096];

ssize_t my_read(int fd, char *ptr){
	if(read_cnt <=0){
		again:
		if((read_cnt=read(fd, read_buf, sizeof(read_buf))) < 0){
			if(errno == EINTR)
				goto again;
			return -1;
		}else
			if(read_cnt == 0)
				return 0;
		read_ptr = read_buf;
	}

	read_cnt --;
	*ptr = *read_ptr;
	read_ptr++;
	return 1;
}
ssize_t _read_line(int fd, void *vptr, size_t max_len){
	ssize_t n, rc;
	char c, *ptr;
	ptr = vptr;
	for(n=1; n < max_len; n++){
		rc = my_read(fd, &c);
		// printf("Log(_read_line): rc=%d\n", rc);
		if(rc == 1){
			*ptr = c;
			ptr++;
			if(c=='\n'){
				break;
			}
		}else
			if(rc == 0){
				*ptr = 0;
				return n-1;
			}else
				return -1;
				
	}
	*ptr = 0;
	return n;
}
ssize_t _Read_line(int fd, void *ptr, size_t max_len){
	ssize_t n;
	if((n=_read_line(fd, ptr, max_len)) <0)
		error("ERROR: readline error");
	return n;
}

int _Select(int nFds, fd_set *readFds, fd_set *writeFds, fd_set *exceptFds, struct timeval *timeout){
	int n;
	n = select(nFds, readFds, writeFds, exceptFds, timeout);
	if(n < 0)
		error("ERROR: select error");
	return n;
}

void _Shutdown(int fd, int how){
	if(shutdown(fd, how)<0){
		error("ERROR: shutdown error");
	}
}

ssize_t _Read(int fd, void *ptr, size_t nbytes){
	ssize_t n;
	n = read(fd, ptr, nbytes);
	if(n == -1)
		error("ERROR(_Read): read error");
	return n;
}

void _Getsockname(int fd, struct sockaddr *sa, socklen_t sa_length_ptr){
	if(getsockname(fd,sa, sa_length_ptr) < 0)
		error("ERROR(_Getsockname): getsockname error");
}

void _Socketpair(int family, int type, int protocol, int *fd){
	if(socketpair(family, type, protocol, fd) < 0)
		errro("(Socketpair) Socket pair error");
}