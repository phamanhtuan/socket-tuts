exe: checkopts sockopts
checkopts:
	gcc -o checkopts checkopts.c ../Lib/wrapper.o ../Lib/error.o -I../Lib -lpthread
sockopts:
	gcc -o sockopts sockopts.c ../Lib/wrapper.o ../Lib/error.o -I../Lib -lpthread
clean: 
	rm -f *.o *.gch checkopts sockopts