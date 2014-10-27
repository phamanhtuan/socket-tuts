#include <wrapper.h>
#include <error.h>

#include <sys/un.h>
#include <sys/errno.h>
#include <sys/signal.h>

#define UNIXSTR_PATH "/tmp/test"
#define MAXLINE 4096
void str_echo(int _connFd);
void handler_child_signal(int);

int main(int argc, char **argv){
	int listen_fd, conn_fd;
	pid_t child_pid;
	signal(SIGCHLD, handler_child_signal);
	socklen_t client_len;
	struct sockaddr_un client_addr, serv_addr;

	listen_fd = _Socket(AF_LOCAL, SOCK_STREAM, 0);

	unlink(UNIXSTR_PATH);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_LOCAL;
	strcpy(serv_addr.sun_path, UNIXSTR_PATH);

	_Bind(listen_fd, (struct sockdaddr *)&serv_addr, sizeof(serv_addr));

	_Listen(listen_fd, 10);
	myLog("Listening ...");
	for( ; ; ){
		client_len = sizeof(client_addr);
		conn_fd = accept(listen_fd, (struct sockdaddr *) &client_addr, &client_len);
		myLog("Accepted client!");
		if(conn_fd < 0)
			if(errno == EINTR)
				continue;
			else
				error("Accept error!");
		if((child_pid = _Fork()) == 0){
			close(listen_fd);
			str_echo(conn_fd);			
			exit(0);
		}
		close(conn_fd);

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
		printf("Child %d terminated\n", pid );
	}
}