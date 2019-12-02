CC = g++
CFLAGS = -g -c
STANDARD = -std=c++11


all: main deadlineratio 

main:  main.o
	$(CC) -o $@ $? 

deadlineratio: deadlineratio.o random.o
	$(CC) -o $@ $? 

main.o:	main.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ main.cpp 

deadlineratio.o: deadlineratio.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ deadlineratio.cpp 
	
random.o: random.cc 
	$(CC) $(CFLAGS) -o $@ random.cc 
	
clean:
	rm *.o *.txt main  main deadlineratio

