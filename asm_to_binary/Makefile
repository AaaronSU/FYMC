CC=gcc
CFLAGS=-std=c99

all: assembleur

#MAIN FILE
assembleur: main.o tools/tools.o parse/parse.o assembly/assembly.o
	$(CC) $(CFLAGS) -O3 $^ -o $@



#TOOLS
tools.o : tools/tools.c
	$(CC) $(CFLAGS) -O3 tools.c -c



#PARSE
parse.o : parse/parse_code/parse_code.o parse/parse_data/parse_data.o
	$(CC) $(CFLAGS) -O3 parse_code/parse_code.o parse_data/parse_data.o -c

parse_code.o : parse/parse_code/parse_code.c
	$(CC) $(CFLAGS) -O3 parse_code.c -c

parse_data.o : parse/parse_data/parse_data.c
	$(CC) $(CFLAGS) -O3 parse_data.c -c



#ASSEMBLY
assembly.o : assembly/assembly.c
	$(CC) $(CFLAGS) -O3 assembly.c -c



#CLEAN
.PHONY: clean
clean:
	rm -f assembleur
	rm -f *.o
	rm -f tools/*.o
	rm -f assembly/*.o
	rm -f parse/*/*.o
	rm -f parse/*.o
