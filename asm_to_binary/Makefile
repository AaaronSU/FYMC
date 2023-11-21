CC=gcc
CFLAGS=-std=c99

all: assembleur

assembleur: main.o tools/tools.o
	$(CC) $(CFLAGS) -O3 $^ -o $@

tools.o : tools/tools.c
	$(CC) $(CFLAGS) -O3 tools.c -c


#CLEAN
.PHONY: clean
clean:
	rm -f assembleur
	rm -f *.o
	rm -f tools/*.o
