all: threaded_binary_tree

threaded_binary_tree: main.o threaded_binary_tree.o
	g++ -o threaded_binary_tree main.o threaded_binary_tree.o

main.o: main.cpp threaded_binary_tree.h
	g++ -c main.cpp

threaded_binary_tree.o: threaded_binary_tree.cpp threaded_binary_tree.h
	g++ -c threaded_binary_tree.cpp

clean:
	rm -f threaded_binary_tree *.o

