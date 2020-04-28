CC=gcc

testLib: testLib.o message.o
		$(CC) testLib.o message.o -o testLib

testLib.o: testLib.c message.h
		$(CC) -c -o testLib.o testLib.c

message.o: message.c message.h
		$(CC) -c -o message.o message.c

clean: 
		rm -f *.o
		rm /tmp/example.fifo
