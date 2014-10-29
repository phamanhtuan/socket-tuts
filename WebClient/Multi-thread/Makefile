exe: web.o 
	gcc -o web web.o ../../Lib/wrapper.o ../../Lib/error.o -lpthread		

web.o: web.c 
	gcc -c web.c -I../../Lib 

clean:
	rm -f *.o *.gch web