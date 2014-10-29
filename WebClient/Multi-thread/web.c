#include <wrapper.h>
#include <error.h>
#include <sys/types.h>

#define MAXFILES 20
#define SERV "80"
#define MAXLINE 4096
struct file {
	char *name;
	char *host;
	int *fd;
	int flag;
	pthread_t tid;
} files[MAXFILES];

#define F_CONNECTING	1
#define F_READING		2
#define F_DONE			4

#define GET_CMD		"GET %s HTTP/1.0 \r\n\r\n"

pthread_mutex_t conn_mutext = PTHREAD_MUTEX_INITIALIZER;
int n_conn, n_files, n_left_to_conn, n_left_to_read;
void * do_get_read(void *vptr);
void homepage(const char *, const char *);
void get_write_cmd(struct file *);

int main(int argc, char **argv){
	int i, n, max_nconn;
	pthread_t tid;
	struct file *fptr;

	if(argc < 5)
		error("Usage: web <#conns> <IPaddr> <homepage> <file1> ...");

	max_nconn = atoi(argv[1]);
	n_files = min(argc-4, MAXFILES);

	for(i = 0; i < n_files; i++){
		files[i].name = argv[i+4];
		files[i].host = argv[2];
		files[i].flag = 0;
	}

	printf("n_files=%d\n", n_files);

	homepage(argv[2], argv[3]);
	n_left_to_conn = n_left_to_read = n_files;
	n_conn = 0;

	while(n_left_to_read > 0){
		while(n_conn < max_nconn && n_left_to_conn > 0){
			for(i = 0; i < n_files; i++)
				if(files[i].flag == 0)
					break;
			if(i == n_files){
				char error_msg[256];
				snprintf(error_msg,"n_left_to_conn = &d but nothing found", n_left_to_conn);
				error(error_msg);
			}
			files[i].flag = F_CONNECTING;
			_Pthread_create(&tid, NULL, &do_get_read, &files[i]);
			files[i].tid = tid;			
			n_conn++;
			n_left_to_conn --;
		}
		// n = pthread_join(0, &tid, (void **) &fptr);
		// if(n != 0){
		// 	errno =n;
		// 	perror("thread_join error");
		// }
		// n_conn--;
		// n_left_to_read--;
		// printf("thread id %d for %s done\n", tid, fptr->name );
	}
	exit(0);
}

void * do_get_read(void *vptr){
	int fd, n;
	char line[MAXLINE];
	struct file *fptr;

	fptr = (struct file *) vptr;

	fd = tcp_connect(fptr->host, SERV);
	fptr->fd = fd;

	printf("do_get_read for %s, fd %d, thread %u\n",fptr->name, fd, pthread_self() );

	write_get_cmd(fptr);

	for( ; ; ){
		n = _Read(fd, line, MAXLINE);
		if(n == 0)
			break;
		printf("read %d bytes from %s\n", n, fptr->name );		
	}
	printf("end-of-file on %s\n", fptr->name);
	close(fd);

	fptr->flag = F_DONE;

	_Pthread_mutex_lock(&conn_mutext);
	n_conn--;
	n_left_to_read--;
	printf("thread id %u for %s done\n", pthread_self(), fptr->name );
	_Pthread_mutex_unlock(&conn_mutext);
	return (fptr);
}

void homepage(const char *host, const char *f_name){
	// myLog("Entered homepage func");
	// printf("host = %s\n", host);
	// printf("f_name = %s\n", f_name);
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

	n = snprintf(line, sizeof(line), GET_CMD, fptr->name);
	_Write_n(fptr->fd, line, n);
	printf("wrote %d bytes for %s\n", n, fptr->name );

	fptr->flag = F_READING;
	// FD_SET(fptr->f_fd, &rset);
	// if(fptr->f_fd > max_fd)
		// max_fd = fptr->f_fd;
}