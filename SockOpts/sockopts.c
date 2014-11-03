#include <wrapper.h>
#include <error.h>
#include <netinet/tcp.h>

int main(int argc, char *argv){
	int 	sock_fd , send_buff;

	int  mss;
	int	opt_len;

	sock_fd = _Socket(AF_INET, SOCK_STREAM, 0);
	mss = 666;
	opt_len = sizeof(mss);
	_Setsockopt(sock_fd, IPPROTO_TCP, TCP_MAXSEG, &mss, opt_len);
	_Getsockopt(sock_fd, IPPROTO_TCP, TCP_MAXSEG, &mss, &opt_len);
	printf("TCP mss = %d\n", mss);

	send_buff = 6000;
	_Setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &send_buff, sizeof(send_buff));

	opt_len = sizeof(send_buff);
	_Getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &send_buff, &opt_len);
	printf("send buffer size = %d\n", send_buff);

	exit(0);
}