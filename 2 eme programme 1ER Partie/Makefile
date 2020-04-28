CC=gcc

testLib2: testLib2.o message.o
		$(CC) testLib2.o message.o -o testLib2

testLib2.o: testLib2.c message.h
		$(CC) -c -o testLib2.o testLib2.c 

message.o: message.c message.h
		$(CC) -c -o message.o message.c

clean: 
		rm -f *.o
		rm /tmp/example.fifo
