#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <wrapper.h>
#include <error.h>

#define SERV_PORT 5000
#define MAXLINE 4096

void doit_thread(void *arg);
void str_echo(int _connFd);
void handler_child_signal(int);
void handler_signal(int);
int listenFd, connFd;
int main(int argc, char ** argv){
	pthread_t tid;
	signal(SIGCHLD, handler_child_signal);
	if(signal(SIGINT, handler_signal) == SIG_ERR)
		printf("Can't catch SIGINT signal!\n");
	pid_t child_pid;
	socklen_t cli_length;
	struct sockaddr_in client_addr, serv_addr;

	listenFd = _Socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);

	_Bind(listenFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	_Listen(listenFd, 5);
	localSockfdPrint(listenFd);
	myLog("Start listening ...");
	for(; ;){
		cli_length = sizeof(client_addr);
		connFd = _Accept(listenFd, (struct sockaddr *) NULL, NULL);
		myLog("Client connected:");
		foreignSockfdPrint(connFd);
		_Pthread_create(&tid, NULL, doit_thread, (void *) connFd);
		// child_pid = _Fork();
		// if(child_pid == 0){
		// 	close(listenFd);
		// 	str_echo(connFd);			
		// 	exit(0);
		// }
		// close(connFd);

	}

}
void str_echo(int sockFd){
	myLog("Entered echo function");
	ssize_t n;
	char buffer[MAXLINE];
	again:
		while((n=read(sockFd, buffer, MAXLINE)) > 0){
			myLog("Received data");
			printf("%s", buffer );
			write(sockFd, buffer, n);
			myLog("Sent back\n");
		}
		if(n<0 && errno == EINTR)
			goto again;
		else
			if(n<0)
				error("ERROR(str_echo): read error");			
}
void handler_child_signal(int signal_no){
	pid_t pid;
	int stat;
	if(signal_no == SIGCHLD){
		printf("Received SIGCHILD signal\n");
		pid = wait(&stat);
		close(connFd);
		printf("Child %d terminated\n", pid );
	}
}
void handler_signal(int signal_no){
	if(signal_no == SIGINT){
		printf("Received SIGINT signal\n");		
		close(connFd);
		exit(0);
	}
}
void doit_thread(void *arg){
	_Pthread_detach(pthread_self());
	str_echo((int) arg);
	close((int)arg);
	return NULL;
}