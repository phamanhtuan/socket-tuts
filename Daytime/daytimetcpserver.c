#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#include <error.h>
#include <wrapper.h>

#define MAXLINE 4096

void doit(int _connFd){
	myLog("Go doit in child process");
	char buffer[MAXLINE + 1];
	time_t ticks;
	ticks = time(NULL);
	snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
	_Write(_connFd, buffer, strlen(buffer));
}
int main(int argc, char ** argv){
	int listenFd, connFd;
	struct sockaddr_in serv_addr;
	
	pid_t pid;

	listenFd = _Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	char ip_addr[] = "10.61.153.55";	
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// inet_pton(AF_INET, "10.61.153.55", &(serv_addr.sin_addr));	
	// inet_aton("10.61.153.55", &serv_addr.sin_addr.s_addr);	
	serv_addr.sin_port = htons(13);

	_Bind(listenFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	_Listen(listenFd, 5);	
	localSockfdPrint(listenFd);
	for(;;){
		connFd = _Accept(listenFd, (struct sockaddr *) NULL, NULL);
		foreignSockfdPrint(connFd);	
		pid = _Fork();
		if(pid == 0){
			close(listenFd);
			doit(connFd);
			close(connFd);
			exit(0);
		}
		myLog("Close connection in parent process");
		close(connFd);
	}
	
	return 0;
}