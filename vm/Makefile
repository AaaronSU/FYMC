CC = gcc
CFLAGS = -Wall -Wconversion -Wextra -g3
FILES = instruction/*.c instruction.h vm.c vm.h

all: run_par

.PHONY:run_par
run_par: mkdir_bin
	$(CC) $(FILES) main.c  -o bin/vm $(CFLAGS) -lm

.PHONY:vm_debug
vm_debug: mkdir_bin
	$(CC) $(FILES) main.c -o bin/vm -DDEBUG $(CFLAGS) -lm
	./bin/vm script.conf

.PHONY:mkdir_bin
mkdir_bin:
	mkdir -p bin

.PHONY:bench
bench: mkdir_bin bench.c
	$(CC) $(FILES) bench.c -o bin/bench -g -O3 -lm
	./bin/bench 

.PHONY:test
test: mkdir_bin test.c
	$(CC) test.c $(FILES) -o bin/test -lcmocka -g3 -O3 $(CFLAGS) -lm
	./bin/test 

.PHONY:clean
clean:
	rm -f bin/*
