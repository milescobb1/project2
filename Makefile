CC = gcc
CFLAGS = -Wall -ansi -g -pedantic
MAIN = mytw
OBJS = mytw.o

all : $(MAIN)

$(MAIN) : $(OBJS) mytw.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mytw.o : mytw.c mytw.h
	$(CC) $(CFLAGS) -c mytw.c

clean : 
	rm *.o $(MAIN) 