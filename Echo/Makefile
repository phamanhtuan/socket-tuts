exe: echotcpserver.o echotcpclient.o
	gcc -o server echotcpserver.o ../Lib/wrapper.o ../Lib/error.o
	gcc -o client echotcpclient.o  ../Lib/wrapper.o ../Lib/error.o

echotcpserver.o: echotcpserver.c
	gcc -c echotcpserver.c -I../Lib 

echotcpclient.o: echotcpclient.c
	gcc -c echotcpclient.c -I../Lib 
clean: 
	rm -f *.o *.gch server client