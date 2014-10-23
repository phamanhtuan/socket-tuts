#include "error.h"

void error(const char *msg){
	perror(msg);
	exit(-1);
}