exe: daytimetcpserver.o daytimetcpclient.o
	gcc -o client daytimetcpclient.o -I../Lib ../Lib/error.o ../Lib/wrapper.o
	gcc -o server daytimetcpserver.o -I../Lib ../Lib/error.o ../Lib/wrapper.o

daytimetcpclient.o: daytimetcpclient.c 
	gcc -c daytimetcpclient.c -I../Lib 

daytimetcpserver.o: daytimetcpserver.c 
	gcc -c daytimetcpserver.c -I../Lib 

clean: 
	rm -f *.o client server *.gch