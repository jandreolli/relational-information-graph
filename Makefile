CC = gcc
CFLAGS += -Wall -Wextra -Werror

SRCS = relations.c
OBJS = $(SRCS:.c=.o)
BINS = $(SRCS:.c=)

.PHONY: all clean

all: $(BINS)

valgrind: $(BINS)
	valgrind --leak-check=full ./$(BINS)

%: %.o 
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(BINS)
