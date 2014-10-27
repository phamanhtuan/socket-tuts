#include <wrapper.h>
#include <error.h>

#include <sys/un.h>
#include <sys/errno.h>
#include <sys/signal.h>

#define UNIXSTR_PATH "/tmp/test"
#define MAXLINE 4096
void str_cli_select(FILE *fp, int sockFd);
int main(int argc, char **argv){
	int sock_fd;
	struct sockaddr_un serv_addr;

	sock_fd = _Socket(AF_LOCAL, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sun_family = AF_LOCAL;	
	strcpy(serv_addr.sun_path, UNIXSTR_PATH);

	_Connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	str_cli(stdin, sock_fd);

	exit(0);
}

void str_cli(FILE *fp, int sockFd){
	myLog("Enter str_cli function");
	char sendLine[MAXLINE], recvLine[MAXLINE];
	while(_Fgets(sendLine, MAXLINE, fp) != NULL){

		_Write_n(sockFd, sendLine, strlen(sendLine));		
		myLog("Sent data");		

		if(_Read_line(sockFd, recvLine, MAXLINE) == 0)
			error("Server terminated");
		myLog("Received data");	
		_Fputs(recvLine, stdout);
	}
}