all: huffman_tree

huffman_tree: main.o huffman_tree.o
	g++ -o huffman_tree main.o huffman_tree.o

main.o: main.cpp huffman_tree.h
	g++ -c main.cpp

huffman_tree.o: huffman_tree.cpp huffman_tree.h
	g++ -c huffman_tree.cpp

clean:
	rm -f huffman_tree *.o

