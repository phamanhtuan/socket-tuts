#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recv_fd){
	myLog("Entered read_fd");
	struct msghdr msg;
	struct iovec iov[1];
	struct cmsghdr *control_message = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	ssize_t n;

	memset(&msg, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(sizeof(int));
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	n = recvmsg(fd, &msg, 0);
	myLog("Received msg");
	printf("n = %d\n", n);
	if(n <= 0)
		return n;	

	for(control_message = CMSG_FIRSTHDR(&msg); 
		control_message != NULL;
		control_message = CMSG_NXTHDR(&msg, control_message) ){
		// myLog("Loop");
		if((control_message->cmsg_level == SOL_SOCKET) &&
			(control_message->cmsg_type == SCM_RIGHTS)){
			// myLog("If");
			*recv_fd = *((int *) CMSG_DATA(control_message));
			myLog("Read_fd: exit (n)");
			return n;
		}
	}
	myLog("Read_fd: exit (-1)");
	return -1;
}

ssize_t _Read_fd(int fd, void *ptr, size_t nbytes, int recv_fd){
	ssize_t n;
	n = read_fd(fd, ptr, nbytes, recv_fd);
	if(n < 0)
		error("read_fd error");

	return n;
}