OBJS	= arguments.o queue.o scheduler.o
SOURCE	= arguments.c queue.c scheduler.c
HEADER	= header.h
DEPS	= makefile
TARGET	= mysched
CC	 = gcc
CFLAGS	 = -Wall -g -c
LFLAGS	 = -lpthread

all: $(TARGET)

$(OBJS): $(SOURCE) $(HEADER) $(DEPS)
	$(CC) $(CFLAGS) $(SOURCE)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *.err *.out