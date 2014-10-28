#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/un.h>

#include <wrapper.h>
#include <error.h>
int main(int argc, char **argv){
	int sock_fd;
	socklen_t len;
	struct sockaddr_un addr1, addr2;

	if(argc != 2){
		error("Usage: unixbind <pathname>");		
	}

	sock_fd = _Socket(AF_LOCAL, SOCK_STREAM, 0);

	unlink(argv[1]);

	bzero(&addr1, sizeof(addr1));

	addr1.sun_family = AF_LOCAL;
	strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path));

	_Bind(sock_fd, (struct sockaddr *)&addr1, SUN_LEN(&addr1));

	len = sizeof(addr2);
	_Getsockname(sock_fd, (struct sockaddr *)&addr2, &len);
	printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);

	exit(0);
}

