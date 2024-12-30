CC = g++
CFLAGS = -Wall -g

TARGET = sstable
OBJS = main.o sstable.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.cpp sstable.h
	$(CC) $(CFLAGS) -c main.cpp

sstable.o: sstable.cpp sstable.h
	$(CC) $(CFLAGS) -c sstable.cpp

clean:
	rm -f $(OBJS) $(TARGET)