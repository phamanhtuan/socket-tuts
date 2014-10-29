#include "web.h"


int main(int argc, char **argv){
	int i, fd, n, max_conn, flags, _error;
	char buf[MAXLINE];
	fd_set rs, ws;

	if(argc < 5)
		error("Usage: web <#conns> <hostname> <homepage> <file1> ...");
	
	max_conn = atoi(argv[1]);

	n_files = min(argc - 4, MAXFILES);
	for(i = 0; i < n_files; i++){
		files[i].f_name = argv[i+4];
		files[i].f_host = argv[2];
		files[i].f_flags = 0;
	}	

	printf("n_files = %d\n", n_files);

	homepage(argv[2], argv[3]);

	FD_ZERO(&rset);
	FD_ZERO(&wset);

	max_fd = -1;
	n_left_to_read = n_left_to_conn = n_files;
	n_conn = 0;

	while(n_left_to_read > 0){
		while(n_conn < max_conn && n_left_to_conn > 0){
			for(i = 0; i < n_files; i++)
				if(files[i].f_flags == 0)
					break;
			if(i == n_files){
				char error_msg[256];
				sprintf(error_msg, "n_left_to_conn = %d but nothing found", n_left_to_conn);
				error(error_msg);
			}
			start_connect(&files[i]);
			n_conn++;
			n_left_to_conn--;
		}

		rs = rset;
		ws = wset;

		n = _Select(max_fd + 1, &rs, &ws, NULL, NULL);

		for(i = 0; i < n_files; i++){
			flags = files[i].f_flags;
			if(flags == 0 || flags & F_DONE)
				continue;
			fd = files[i].f_fd;
			if(flags & F_CONNECTING && 
				(FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))){
				n = sizeof(_error);
				if(getsockopt(fd, SOL_SOCKET, SO_ERROR, &_error, &n) < 0 || _error != 0){
					char error_msg[256];
					sprintf(error_msg,"Nonblockking connect for %s failed", files[i].f_name );
					error(error_msg);
				}
			
				printf("Connection established for %s\n", files[i].f_name );
				write_get_cmd(&files[i]);
			}else{
				if(flags & F_READING && FD_ISSET(fd, &rs)){
					n = _Read(fd, buf, sizeof(buf));
					if(n == 0){
						printf("End-of-file on %s\n", files[i].f_name );
						close(fd);
						files[i].f_flags = F_DONE;
						FD_CLR(fd, &rset);
						n_conn--;
						n_left_to_read--;						
					}else{
						printf("Read %d bytes from %s\n", n, files[i].f_name);
					}
				}
			}
		}
	}
	exit(0);
}