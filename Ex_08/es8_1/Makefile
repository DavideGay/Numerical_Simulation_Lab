CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o psi.o
	$(CC) random.o psi.o main.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
psi.o : psi.cpp psi.h
	$(CC) -c psi.cpp -o psi.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out
