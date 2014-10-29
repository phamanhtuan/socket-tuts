#include "web.h"

void homepage(const char *host, const char *f_name){
	myLog("Entered homepage func");
	printf("host = %s\n", host);
	printf("f_name = %s\n", f_name);
	int fd, n;
	char line[MAXLINE];

	fd = tcp_connect(host, SERV);

	n = snprintf(line, sizeof(line), GET_CMD, f_name);

	_Write_n(fd, line, n);

	for( ; ;){
		n = _Read(fd, line, MAXLINE);
		if(n == 0)
			break;
		printf("read %d bytes of homepage\n", n);				
	}
	printf("end-of-file on homepage\n");
	close(fd);
}
void write_get_cmd(struct file *fptr){
	int n;
	char line[MAXLINE];

	n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
	_Write_n(fptr->f_fd, line, n);
	printf("wrote %d bytes for %s\n", n, fptr->f_name );

	fptr->f_flags = F_READING;
	FD_SET(fptr->f_fd, &rset);
	if(fptr->f_fd > max_fd)
		max_fd = fptr->f_fd;
}
void start_connect(struct file *fptr){
	int fd, flags, n;
	struct addrinfo *ai;

	ai = _Host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
	fd = _Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	fptr->f_fd = fd;
	printf("Start_connect for %s, fd %d\n", fptr->f_name, fd );

	flags = _Fcntl(fd, F_GETFL, 0);
	_Fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	n = connect(fd, ai->ai_addr, ai->ai_addrlen);
	if(n < 0){
		if(errno != EINPROGRESS)
			error("Nonblocking connect error");	
		fptr->f_flags = F_CONNECTING;
		FD_SET(fd, &rset);
		FD_SET(fd, &wset);

		if(fd > max_fd)
			max_fd = fd;
	}else
		if(n >= 0)
			write_get_cmd(fptr);
}