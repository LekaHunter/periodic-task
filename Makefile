CC=gcc
OPTION=-g -c -o

testEnvCmd: testEnvCmd.o message.o
	$(CC) -g testEnvCmd.o message.o -o testEnvCmd
	
testEnvCmd.o:
	$(CC) $(OPTION) testEnvCmd.o testEnvCmd.c
	
message.o: message.c message.h
	$(CC) $(OPTION) message.o message.c

clean: 
	rm -f *.o
