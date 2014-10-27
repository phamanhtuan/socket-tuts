#include <wrapper.h>
#include <error.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFFSIZE 10

int my_open(const char *pathname, int mode);
int main(int argc, char **argv){
	int fd, n;
	char buff[BUFFSIZE];

	if(argc != 2)
		error("Usage: mycat <pathname>");
	fd = my_open(argv[1], O_RDONLY);
	if(fd < 0)
		error("Cannot open file");

	while((n = _Read(fd, buff, BUFFSIZE)) > 0)
		_Write(STDOUT_FILENO, buff, n);

	exit(0);
}

int my_open(const char *pathname, int mode){
	int fd, sock_fd[2], status;
	pid_t child_pid;
	char c, arg_sock_fd[10], arg_mode[10];
	_Socketpair(AF_LOCAL, SOCK_STREAM, 0, sock_fd);
	if((child_pid = _Fork()) == 0){
		close(sock_fd[0]);
		snprintf(arg_sock_fd, sizeof(arg_sock_fd), "%d", sock_fd[1]);
		snprintf(arg_mode, sizeof(arg_mode), "%d", mode);

		execl("./openfile", "openfile", arg_sock_fd, pathname, arg_mode, (char *)NULL);
		error("execl error");
	}
	close(sock_fd[1]);
}