CC=gcc
TARGET=projet

all=$(TARGET)

$(TARGET): periodic.o message.o
	$(CC) periodic.o message.o -o $(TARGET)

periodic.o: periodic.c message.h
	$(CC) -c -o periodic.o periodic.c

message.o: message.c message.h
	$(CC) -c -o message.o message.c

clean: 
	rm -f *.o
	rm -f $(TARGET)
