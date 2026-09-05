CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = scheduler

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c scheduler.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) rate_lagi.out edf_lagi.out
