exe: mycat openfile
		
mycat: mycat.o read_fd.c
	gcc -o mycat mycat.o read_fd.c ../../Lib/error.o ../../Lib/wrapper.o -lpthread
mycat.o: mycat.c read_fd.c
	gcc -c mycat.c read_fd.c -I../../Lib 	

openfile: openfile.o write_fd.c
	gcc -o openfile openfile.o write_fd.c ../../Lib/error.o ../../Lib/wrapper.o -lpthread
openfile.o: openfile.c write_fd.c
	gcc -c openfile.c write_fd.c -I../../Lib

clean: 
	rm -f *.o mycat openfile