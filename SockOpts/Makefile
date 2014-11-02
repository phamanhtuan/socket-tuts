exe: sockopts checksockopts
checksockopts:
	gcc -o checksockopts checksockopts.c -I../Lib ../Lib/wrapper.o ../Lib/error.o -lpthread	
sockopts: sockopts.c
	gcc -o sockopts sockopts.c -I../Lib ../Lib/wrapper.o ../Lib/error.o -lpthread
clean: 
	rm -f *.o *.gch sockopts