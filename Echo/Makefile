exe: echotcpserver.o echotcpclient.o tcpserverselect.o
	gcc -o server echotcpserver.o ../Lib/wrapper.o ../Lib/error.o -lpthread
	gcc -o client echotcpclient.o  ../Lib/wrapper.o ../Lib/error.o -lpthread
	gcc -o sl_server tcpserverselect.o  ../Lib/wrapper.o ../Lib/error.o -lpthread

echotcpserver.o: echotcpserver.c
	gcc -c echotcpserver.c -I../Lib 

echotcpclient.o: echotcpclient.c
	gcc -c echotcpclient.c -I../Lib 
tcpserverselect.o: tcpserverselect.c
	gcc -c tcpserverselect.c -I../Lib	
clean: 
	rm -f *.o *.gch server client