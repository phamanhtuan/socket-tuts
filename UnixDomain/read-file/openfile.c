#include <wrapper.h>
#include <error.h>
#include <errno.h>

int main(int argc, char ** argv){
	myLog("Openfile started");
	int fd;
	if(argc != 4)
		error("Usage: openfile <socfd#> <filename> <mode>");
	fd = open(argv[2], atoi(argv[3]));
	if(fd < 0)
		exit(errno > 0? errno: 255);
	myLog("writing data");
	while(write_fd(atoi(argv[1]), "", 1, fd) < 0)
		exit(errno > 0? errno:255);
	
	exit(0);
}