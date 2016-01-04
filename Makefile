CC = g++
TARGET:=prolog++
CFLAGS   = -Iincludes/. -pedantic -pedantic-errors
LFLAGS   = -Wall -I. -lm
OBJS:= main.o atom.o parser.o predicate.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.cpp prolog++.h
	$(CC) -c main.cpp

atom.o: atom.cpp prolog++.h
	$(CC) -c atom.cpp

parser.o: parser.cpp prolog++.h
	$(CC) -c parser.cpp

predicate.o: predicate.cpp prolog++.h
	$(CC) -c predicate.cpp

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
