#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int send_fd){
	printf("Entered write_fd func\n");
	struct msghdr msg;
	struct iovec iov[1];

	struct cmsghdr *control_message = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	
	memset(&msg, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(sizeof(int));

	control_message = CMSG_FIRSTHDR(&msg);
	control_message->cmsg_level = SOL_SOCKET;
	control_message->cmsg_type = SCM_RIGHTS;
	control_message->cmsg_len = CMSG_LEN(sizeof(int));

	*((int*) CMSG_DATA(control_message)) = send_fd;	

	printf("Exit write_fd\n");
	return (sendmsg(fd, &msg, 0));


}