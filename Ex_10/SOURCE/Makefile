CC = g++
CCMPI = mpicxx
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o city.o path.o population.o
	$(CCMPI) random.o city.o path.o population.o main.o -o main.exe $(CFLAGS)
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
city.o : city.cpp city.h
	$(CC) -c city.cpp -o city.o $(CFLAGS)
path.o : path.cpp path.h
	$(CC) -c path.cpp -o path.o $(CFLAGS)
population.o : population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out
run : main.exe
	mpiexec -np 4 ./main.exe