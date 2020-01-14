CC = g++
CFLAGS = -g -c
STANDARD = -std=c++11


all: main deadlineratio stationary download

main:  main.o
	$(CC) -o $@ $? 

#deadlineratio: deadlineratio.o random.o
#	$(CC) -o $@ $? 

deadlineratio: deadlineratio.o
	$(CC) -o $@ $? 

deadlineratio.o: deadlineratio.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ deadlineratio.cpp 

stationary: stationary.o
	$(CC) -o $@ $? 	

stationary.o: stationary.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ stationary.cpp 

download: download.o
	$(CC) -o $@ $? 	

download.o: download.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ download.cpp 		

main.o:	main.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ main.cpp 


		
#deadlineratio.o: deadlineratio.cpp
#	$(CC) $(STANDARD) $(CFLAGS) -o $@ deadlineratio.cpp 
	
random.o: random.cc 
	$(CC) $(CFLAGS) -o $@ random.cc 
	
clean:
	rm *.o *.txt main  main deadlineratio stationary download

