CC = gcc
CFLAGS = -Wall -Wconversion -Wextra -g3
FILES = instruction/*.c instruction.h vm.c vm.h

all: run_par

.PHONY:run_par
run_par: mkdir_bin
	$(CC) $(FILES) main.c  -o bin/vm $(CFLAGS)
	./bin/vm script.conf

.PHONY:vm_debug
vm_debug: mkdir_bin
	$(CC) $(FILES) main.c -o bin/vm -DDEBUG $(CFLAGS)
	./bin/vm script.conf

.PHONY:mkdir_bin
mkdir_bin:
	mkdir -p bin

.PHONY:bench
bench: bench.c
	$(CC) $(FILES) bench.c -o bin/bench -lm -g -O3
	./bin/bench 

.PHONY:test
test: test.c
	$(CC) test.c $(FILES) -o bin/test -lcmocka -lm -g3 -O3 $(CFLAGS)
	./bin/test 

.PHONY:clean
clean:
	rm -f bin/*