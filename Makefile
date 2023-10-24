CC=gcc
CFLAGS=

mysh: shell.c interpreter.c shellmemory.c pcb.c ready_queue.c
	$(CC) $(CFLAGS) -D X=$(framesize) -D Y=${varmemsize} -c -g shell.c interpreter.c shellmemory.c pcb.c ready_queue.c page.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o pcb.o ready_queue.o page.o

clean: 
	rm mysh; rm *.o