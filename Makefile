CC = gcc
CFLAGS = -Wall -ansi -g -pedantic
MAIN = fw
OBJS = fw.o

all : $(MAIN)

fw : all

$(MAIN) : $(OBJS) fw.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

fw.o : fw.c fw.h
	$(CC) $(CFLAGS) -c fw.c

clean : 
	rm *.o $(MAIN) 