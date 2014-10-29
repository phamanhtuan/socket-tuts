#ifndef __WEB_H__
#define __WEB_H__

#include <wrapper.h>
#include <error.h>
#include <fcntl.h>
#define MAXFILES 20
#define SERV "80"
#define MAXLINE 4096

struct file{
	char *f_name;
	char *f_host;
	int f_fd;
	int f_flags;
} files[MAXFILES];

#define F_CONNECTING	1
#define F_READING		2
#define F_DONE			4

#define GET_CMD		"GET %s http/1.0\r\n\r\n"

int n_conn, n_files, n_left_to_conn, n_left_to_read, max_fd;
fd_set rset, wset;

void homepage(const char *, const char *);
void get_write_cmd(struct file *);
void start_connect(struct file *);

#endif