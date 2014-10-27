#include <signal.h>
#include <unistd.h>

#include <wrapper.h>
#include <error.h>

#define MAXLINE 4096
int main(int argc, char **argv){
	int i, maxi, max_fd, listen_fd, conn_fd, sock_fd;
	int n_ready, client[FD_SETSIZE];
	ssize_t n;
	fd_set r_set, all_set;
	char buffer[MAXLINE];

	socklen_t client_len;
	struct sockaddr_in client_addr, serv_addr;

	listen_fd = _Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);
	
	_Bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	_Listen(listen_fd, 5);

	max_fd = listen_fd;
	maxi = -1;
	for(i = 0; i<FD_SETSIZE; i++)
		client[i]=-1;
	FD_ZERO(&all_set);
	FD_SET(listen_fd, &all_set);
	localSockfdPrint(listen_fd);
	for( ; ; ){
		r_set = all_set;
		n_ready = _Select(max_fd+1, &r_set, NULL, NULL, NULL);

		if(FD_ISSET(listen_fd, &r_set)){
			client_len = sizeof(client_addr);
			conn_fd = _Accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
			myLog("Accepted client");
			foreignSockfdPrint(conn_fd);
			for(i=0; i<FD_SETSIZE; i++)
				if(client[i]<0){
					client[i] = conn_fd;
					myLog("Set fd for client");
					break;
				}
			if(i==FD_SETSIZE)
				error("Too mamy clients");
			FD_SET(conn_fd, &all_set);
			if(conn_fd > max_fd)
				max_fd = conn_fd;
			if(i>maxi)
				maxi = i;
			// printf("main: n_ready=%d\n", --n_ready);			
			n_ready--;
			if(n_ready<=0)
				continue;
		}

		for(i=0; i<=maxi; i++){
			sock_fd = client[i];
			if(sock_fd<0)
				continue;
			if(FD_ISSET(sock_fd, &r_set)){
				n = _Read(sock_fd, buffer, MAXLINE);
				// printf("main: n=%d\n", n);
				if(n == 0){
					close(sock_fd);
					FD_CLR(sock_fd, &all_set);
					client[i] = -1;				
				}else			
					_Write_n(sock_fd, buffer, n);
			
				n_ready--;
				if(n_ready<=0)
					break;
			}
		}
	}
}