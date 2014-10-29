exe: unixbind unixserv unixclient

unixbind: unixbind.o
	gcc -o unixbind unixbind.o ../../Lib/error.o ../../Lib/wrapper.o -lpthread
unixbind.o: unixbind.c 
	gcc -c unixbind.c -I../../Lib 
unixserv: unixserv.o
	gcc -o unixserv unixserv.o ../../Lib/error.o ../../Lib/wrapper.o -lpthread
unixserv.o: unixserv.c 
	gcc -c unixserv.c -I../../Lib 	
unixclient: unixclient.o
	gcc -o unixclient unixclient.o ../../Lib/error.o ../../Lib/wrapper.o -lpthread
unixclient.o: unixclient.c 
	gcc -c unixclient.c -I../../Lib 			
clean: 
	rm -f *.o unixbind unixclient unixserv mycat