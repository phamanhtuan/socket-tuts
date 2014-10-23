#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sockFd, newsockFd, portNo, clientLength, n;	
	char buffer[256];
	struct sockaddr_in serv_addr, client_addr;

	if(argc < 2){
		fprintf(stderr, "ERROR: no port provided");
		exit(1);
	}
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd <0)
		error("ERROR openning socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portNo = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portNo);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
		error("ERROR on binding");
	listen(sockFd, 5);

	clientLength = sizeof(client_addr);
	newsockFd =  accept(sockFd, (struct sockaddr *) &client_addr, &clientLength);

	if(newsockFd < 0)
		error("ERROR on accept");

	bzero(buffer, 256);
	n = read(newsockFd, buffer, 255);
	if(n<0)
		error("ERRO reading from socket");
	printf("Here is the message: %s", buffer);	

	n = write(newsockFd, "I got your message", 18);
	if(n<0)
		error("ERROR writing to socket");
	return 0;
}