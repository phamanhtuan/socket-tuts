error.o: error.c error.h
	gcc -c error.c error.h
	gcc -c wrapper.c wrapper.h error.h

wrapper.o: wrapper.c wrapper.h error.h
	gcc -c wrapper.c wrapper.h error.h

clean: 
	rm -f *.o *.gch