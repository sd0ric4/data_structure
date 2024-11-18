all: binary_tree

binary_tree: main.o binary_tree.o
	g++ -o binary_tree main.o binary_tree.o

main.o: main.cpp binary_tree.h
	g++ -c main.cpp

binary_tree.o: binary_tree.cpp binary_tree.h
	g++ -c binary_tree.cpp

clean:
	rm -f binary_tree *.o

