CC = g++
CFLAGS  = -O3 #-Wall -g -pg -fopenmp -O2 -O3

all : Main
Main : output/Main.o output/tests.o output/figures.o output/fitting.o output/latticeclass.o output/sitearrayclass.o  output/Util.o
	$(CC) $(CFLAGS) -o Main output/Main.o output/tests.o output/figures.o output/fitting.o output/latticeclass.o output/sitearrayclass.o output/Util.o -lpython3.10 -lgsl -lgslcblas -lm
output/Main.o: src/Main.cpp include/figures.h include/fitting.h include/tests.h include/latticeclass.h include/sitearrayclass.h include/Util.h
	$(CC) $(CFLAGS) -c src/Main.cpp -o output/Main.o
output/tests.o: src/tests.cpp  include/latticeclass.h
	$(CC) $(CFLAGS) -c src/tests.cpp -o output/tests.o
output/figures.o: src/figures.cpp  include/fitting.h include/Util.h
	$(CC) $(CFLAGS) -c src/figures.cpp -o output/figures.o
output/fitting.o: src/fitting.cpp  
	$(CC) $(CFLAGS) -c src/fitting.cpp -o output/fitting.o
output/latticeclass.o: src/latticeclass.cpp include/sitearrayclass.h include/Util.h
	$(CC) $(CFLAGS) -c src/latticeclass.cpp -o output/latticeclass.o
output/sitearrayclass.o: src/sitearrayclass.cpp  include/Util.h
	$(CC) $(CFLAGS) -c src/sitearrayclass.cpp -o output/sitearrayclass.o
output/Util.o: src/Util.cpp 
	$(CC) $(CFLAGS) -c src/Util.cpp -o output/Util.o
clean:
	rm -f output/*.o Main