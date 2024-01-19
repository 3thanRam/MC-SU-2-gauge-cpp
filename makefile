all : Main
Main : Main.o plotting/graph.o classes/lattice.o classes/a_class.o misc/Util.o
	g++ -o Main Main.o plotting/graph.o classes/lattice.o classes/a_class.o misc/Util.o -lpython3.10
Main.o: Main.cpp plotting/graph.h classes/lattice.h 
	g++ -c Main.cpp -o Main.o
plotting/graph.o: plotting/graph.cpp classes/lattice.h misc/Util.h
	g++ -c plotting/graph.cpp -o plotting/graph.o
classes/lattice.o: classes/lattice.cpp classes/a_class.h misc/Util.h
	g++ -c classes/lattice.cpp -o classes/lattice.o
classes/a_class.o: classes/a_class.cpp misc/Util.h
	g++ -c classes/a_class.cpp -o classes/a_class.o
misc/Util.o: misc/Util.cpp 
	g++ -c misc/Util.cpp -o misc/Util.o
