exe: web.o homepage.o
	gcc -o web web.o homepage.o ../../Lib/wrapper.o ../../Lib/error.o -lpthread		

web.o: web.c 
	gcc -c web.c -I../../Lib 
homepage.o: homepage.c
	gcc -c homepage.c -I../../Lib
clean: 
	rm -f *.o *.gch web