all: superserver.exe

superserver.exe: superserver.o init_services.o
	gcc -o superserver.exe superserver.o init_services.o 

init_services.o: init_services.c service.h
	gcc -c init_services.c 

superserver.o: superserver.c service.h checkers.h
	gcc -c superserver.c 

.PHONY: clean

clean: 
	rm *.o *.exe