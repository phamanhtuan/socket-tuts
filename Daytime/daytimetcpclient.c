#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <wrapper.h>

#define MAXLINE 4096


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
	serv_addr.sin_port = htons(13);
	myLog("inet_pton");
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0){
		char error_msg[256];
		sprintf(error_msg, "ERROR: inet_pton for %s", argv[1]);
		error(error_msg);
	}
	myLog("connecting");	
	_Connect(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	while((n=read(sockFd, recvLine, MAXLINE)) > 0){
		recvLine[n] = 0;
		if(fputs(recvLine, stdout) == EOF)
			error("ERROR: fputs error");
	}

	if(n<0)
		error("ERROR read");
	
	return 0;

}