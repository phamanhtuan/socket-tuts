#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sockFd, portNo, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	if(argc < 3){
		fprintf(stderr, "usage %s hostname port", argv[0] );
		exit(1);
	}
	portNo = atoi(argv[2]);
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0)
		error("ERROR openning socket");
	server = gethostbyname(argv[1]);
	if(server == NULL){
		fprintf(stderr, "ERROR no such host");
		exit(0);
	}	

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNo);

	if(connect(sockFd, &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	printf("Please enter the message");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockFd, buffer, strlen(buffer));

	if(n<0)
		error("ERROR writing to socket");

	bzero(buffer, 256);
	n = read(sockFd, buffer, 256);

	if(n<0)
		error("ERROR reading from socket");
	printf("%s", buffer );

	return 0;
}