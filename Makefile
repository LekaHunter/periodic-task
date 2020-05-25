CC=gcc
OPTION=-g -c -o
OPT=-g -o

create_file_o: compil_period1 compil_period compil_periodic1 compil_periodic compile_launcher_daemon

compile_message: lib/message.c include/message.h
	$(CC) $(OPTION) lib/message.o lib/message.c
	
compile_periodiclib: lib/periodicLib.c include/periodicLib.h
	$(CC) $(OPTION) lib/periodicLib.o lib/periodicLib.c
	
compile_periodlib: lib/periodLib.c include/periodLib.h
	$(CC) $(OPTION) lib/periodLib.o lib/periodLib.c
	
compil_period1: period.c
	$(CC) $(OPTION) period.o period.c
	
compil_period: period.o compile_message compile_periodlib
	$(CC) -g period.o lib/message.o lib/periodLib.o -o period
	
compil_periodic1: periodic.c
	$(CC) $(OPTION) period.o period.c
	
compil_periodic: periodic.o compile_message compile_periodiclib
	$(CC) -g periodic.o lib/message.o lib/periodicLib.o -o periodic
	
compile_launcher_daemon: launch_daemon.c
	$(CC) $(OPT) launch_daemon launch_daemon.c
	

clean: 
	rm -f period
	rm -f periodic
	rm -f launch_daemon
	rm -rf /tmp/period
	rm -f /tmp/period.err
	rm -f /tmp/period.fifo
	rm -f /tmp/period.out
	rm -f *.o
