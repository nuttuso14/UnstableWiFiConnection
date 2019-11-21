CC = g++
CFLAGS = -g -c
STANDARD = -std=c++11


all: main 

main: main.o
	$(CC) -o $@ $? 

main.o:	main.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ main.cpp 

clean:
	rm *.o *.txt main 

