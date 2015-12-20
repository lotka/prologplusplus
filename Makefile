CC = g++
TARGET:=prolog++
CFLAGS   = -Iincludes/. -pedantic -pedantic-errors
LFLAGS   = -Wall -I. -lm
OBJS:= prolog++.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

prolog++.o: prolog++.cpp prolog++.h
	$(CC) -c prolog++.cpp

.PHONY: clean
clean:
	rm -f *.o $(TARGET)