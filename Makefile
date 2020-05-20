CC=gcc
OPTION=-g -c -o

testeLib: testLib.o message.o
	$(CC) -g testLib.o message.o -o testLib
	
testLib.o:
	$(CC) $(OPTION) testLib.o testLib.c
	
message.o: message.c message.h
	$(CC) $(OPTION) message.o message.c

clean: 
	rm -f *.o
