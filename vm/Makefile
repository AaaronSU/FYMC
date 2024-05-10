CC = gcc
CFLAGS = -Wall -Wconversion -Wextra -g3
FILES = main.c instruction.c instruction.h vm.c vm.h

all: run_par

.PHONY:run_par
run_par: mkdir_bin
	$(CC) $(FILES) -o bin/vm $(CFLAGS)
	./bin/vm script.conf

.PHONY:vm_debug
vm_debug: mkdir_bin
	$(CC) $(FILES) -o bin/vm -DDEBUG $(CFLAGS)
	./bin/vm script.conf

.PHONY:verify_as
verify_as: mkdir_bin
	$(CC) as.c -o bin/as -Werror -O3 $(CFLAGS)
	./bin/as reference/hello_world.asm
	python3 script/compare.py output.archyb reference/hello_world.archyb
	./bin/as reference/fibonacci.asm
	python3 script/compare.py output.archyb reference/fibonacci.archyb
	./bin/as reference/dotprod_u64.asm

.PHONY:mkdir_bin
mkdir_bin:
	mkdir -p bin

.PHONY:bench
bench: bench.c
	$(CC)  instruction.h vm.h instruction.c vm.c bench.c -o bin/bench -lm -g -O3
	./bin/bench 

.PHONY:check
check: test.c
	$(CC) test.c instruction.c vm.c -o bin/test -lcmocka -lm -g3 -O3 $(CFLAGS)
	./bin/test 

.PHONY:clean
clean:
	rm -f bin/*