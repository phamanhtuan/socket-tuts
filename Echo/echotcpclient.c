#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>

#include <wrapper.h>
#include <error.h>

#define MAXLINE 4096
void str_cli(FILE *fp, int sockFd);
void str_cli_select(FILE *fp, int sockFd);
void handler_signal(int );
void handler_sigpipe(int );
int main(int argc, char ** argv){
	if(signal(SIGINT, handler_signal) == SIG_ERR)
		printf("Can't catch SIGINT signal!\n");
	signal(SIGPIPE, handler_signal);
	int sockFd;
	struct sockaddr_in server_addr;

	if(argc != 2){
		char msg[256];
		sprintf(msg, "ERROR: usage %s <IP address>", argv[0]);
		error(msg);
	}
	sockFd = _Socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	_Connect(sockFd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	myLog("Connected to server");
	// str_cli(stdin, sockFd);
	str_cli_select(stdin, sockFd);
	exit(0);
}

void str_cli(FILE *fp, int sockFd){
	myLog("Enter str_cli function");
	char sendLine[MAXLINE], recvLine[MAXLINE];
	while(_Fgets(sendLine, MAXLINE, fp) != NULL){

		_Write_n(sockFd, sendLine, strlen(sendLine));		
		myLog("Sent data");
		sleep(1);
		_Write_n(sockFd, sendLine+1, strlen(sendLine)-1);		
		myLog("Sent 2nd-data");

		if(_Read_line(sockFd, recvLine, MAXLINE) == 0)
			error("Server terminated");
		myLog("Received data");	
		_Fputs(recvLine, stdout);
	}
}
void handler_signal(int signal_no){
	if(signal_no == SIGINT){
		printf("Received INT signal\n");
		exit(0);
	}
}
void handler_sigpipe(int signal_no){
	if(signal_no == SIGPIPE){
		printf("Received PIPE signal\n");
		exit(0);
	}
}
void str_cli_select(FILE *fp, int sockFd){
	int maxFdp1, stdineof;
	fd_set rset;
	char sendLine[MAXLINE], recvLine[MAXLINE];

	stdineof  = 0;
	FD_ZERO(&rset);
	for(; ;){
		FD_SET(fileno(fp), &rset);
		FD_SET(sockFd, &rset);
		maxFdp1 = max(fileno(fp), sockFd) + 1;
		_Select(maxFdp1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(sockFd, &rset)){
			if(_Read_line(sockFd, recvLine, MAXLINE) == 0)
				if(stdineof == 1)
					return;
				else
					error("Server terminated");
			_Fputs(recvLine, stdout);
		}
		if(FD_ISSET(fileno(fp), &rset)){
			if(_Fgets(sendLine, MAXLINE, fp) == NULL){
				stdineof = 1;
				_Shutdown(sockFd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;				
			}
			_Write_n(sockFd, sendLine, strlen(sendLine));
		}
	}
}

