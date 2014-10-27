exe: unixbind unixserv unixclient

unixbind: unixbind.o
	gcc -o unixbind unixbind.o -I../Lib ../Lib/error.o ../Lib/wrapper.o	
unixbind.o: unixbind.c 
	gcc -c unixbind.c -I../Lib 
unixserv: unixserv.o
	gcc -o unixserv unixserv.o -I../Lib ../Lib/error.o ../Lib/wrapper.o	
unixserv.o: unixserv.c 
	gcc -c unixserv.c -I../Lib 	
unixclient: unixclient.o
	gcc -o unixclient unixclient.o -I../Lib ../Lib/error.o ../Lib/wrapper.o	
unixclient.o: unixclient.c 
	gcc -c unixclient.c -I../Lib 		

clean: 
	rm -f *.o unixbind unixclient unixserv