CC=gcc 
TARGET=test

all=$(TARGET)

$(TARGET): testLib.o message.o
		$(CC) testLib.o message.o -o $(TARGET)

testLib.o: testLib.c message.h
		$(CC) -c -o testLib.o testLib.c 

message.o: message.c message.h
		$(CC) -c -o message.o message.c

clean: 
		rm -f *.o
		rm /tmp/example.fifo
		
mrproper: clean
		rm -f $(TARGET)
