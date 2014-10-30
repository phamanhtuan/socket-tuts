#include <wrapper.h>
#include <error.h>
#include <netinet/tcp.h>

int main(int argc, char **argv){
	int sock_fd, mss, send_buff;
	socklen_t optlen;

	sock_fd = _Socket(AF_INET, SOCK_STREAM, 0);

	optlen = sizeof(mss);

	_Getsockopt(sock_fd, IPPROTO_TCP, TCP_MAXSEG, &mss, &optlen);
	printf("TCP mss = %d\n", mss);

	send_buff = 3000;
	optlen = sizeof(send_buff);
	_Setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &send_buff, sizeof(send_buff));

	
	_Getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &send_buff, &optlen);;
	printf("send_buff = %d\n",  send_buff);
	exit(0);
}