CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c instruction.c init.c
OBJS = $(SRCS:.c=.o)
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
