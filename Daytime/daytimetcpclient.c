#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <error.h>
#include <wrapper.h>

#define MAXLINE 4096

int connect_nonb(int sock_fd, const struct sockaddr *sa_ptr, socklen_t sa_len, int n_sec);

int main(int argc, char **argv){
	int sockFd, n;
	struct sockaddr_in serv_addr;	

	char recvLine[MAXLINE +1];
	if(argc != 2){
		char error_msg[256];
		sprintf(error_msg, "Usage: %s <IP Address>", argv[0]);
		error(error_msg);
	}

	sockFd = _Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9000);
	myLog("inet_pton");
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0){
		char error_msg[256];
		sprintf(error_msg, "ERROR: inet_pton for %s", argv[1]);
		error(error_msg);
	}
	myLog("connecting");	
	// _Connect(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	int res;
	res = connect_nonb(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr), 0);
	printf("res=%d\n", res );
	while((n=read(sockFd, recvLine, MAXLINE)) > 0){
		recvLine[n] = 0;
		if(fputs(recvLine, stdout) == EOF)
			error("ERROR: fputs error");
	}

	if(n<0)
		error("ERROR read");
	
	return 0;

}

int connect_nonb(int sock_fd, const struct sockaddr *sa_ptr, socklen_t sa_len, int n_sec){
	int flags, n, error_;
	socklen_t len;
	fd_set rset, wset;
	struct timeval tval;

	flags = _Fcntl(sock_fd, F_GETFL, 0);
	_Fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

	error_ = 0;

	n = connect(sock_fd, sa_ptr, sa_len);
	if(n < 0)
		if(errno != EINPROGRESS)
			return -1;

	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sock_fd, &rset);
	wset = rset;
	tval.tv_sec = n_sec;
	tval.tv_usec = 0;
	n = _Select(sock_fd+1, &rset, &wset, NULL, n_sec? &tval: NULL);
	printf("n=%d\n",n );
	if(n == 0){
		close(sock_fd);
		errno = ETIMEDOUT;
		return -1;
	}

	if(FD_ISSET(sock_fd, &rset) || FD_ISSET(sock_fd, &wset)){
		len = sizeof(error_);
		if(getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &error_, &len))
			return -1;
	}else{
		error("Select error: sock_fd not set");
	}
	done:
		_Fcntl(sock_fd, F_SETFL, flags);
		if(error_){
			close(sock_fd);
			errno = error_;
			return -1;
		}
		return 0;
}