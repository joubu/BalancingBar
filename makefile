CC=gcc
CPPFLAGS=-o
all : balancing_bar
balancing_bar : balancing_bar.o
	$(CC) -o balancing_bar balancing_bar.o -lcng -lSDL -L./CNG -lpthread -g
balancing_bar.o : balancing_bar.c
	$(CC) -c balancing_bar.c -c -o balancing_bar.o -g
clean : 
	rm -f *.o

