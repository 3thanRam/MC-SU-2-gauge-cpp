all : Main
Main : output/Main.o output/tests.o output/graphs.o output/latticeclass.o output/sitearrayclass.o  output/Util.o
	g++ -pg -o Main output/Main.o output/tests.o output/graphs.o output/latticeclass.o output/sitearrayclass.o output/Util.o -lpython3.10
output/Main.o: src/Main.cpp include/graphs.h include/tests.h include/latticeclass.h include/sitearrayclass.h include/Util.h
	g++ -pg -c src/Main.cpp -o output/Main.o
output/tests.o: src/tests.cpp  include/latticeclass.h
	g++ -pg -c src/tests.cpp -o output/tests.o
output/graphs.o: plotting/graphs.cpp  include/Util.h
	g++ -pg -c plotting/graphs.cpp -o output/graphs.o
output/latticeclass.o: src/latticeclass.cpp include/sitearrayclass.h include/Util.h
	g++ -pg -c src/latticeclass.cpp -o output/latticeclass.o
output/sitearrayclass.o: src/sitearrayclass.cpp  include/Util.h
	g++ -pg -c src/sitearrayclass.cpp -o output/sitearrayclass.o
output/Util.o: src/Util.cpp 
	g++ -pg -c src/Util.cpp -o output/Util.o