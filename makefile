CC=g++
CFLAGS= -O2
OBJFLAGS= -c
SDL= -l SDL2
all: main.o rsdl.o calc.o inputHandling.o gui.o
	$(CC) $(CFLAGS) main.o rsdl.o calc.o inputHandling.o gui.o  $(SDL) -o qbby.out
rsdl.o: rsdl.h rsdl.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) rsdl.cpp
inputHandling.o: qbby.h inputHandling.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) inputHandling.cpp
calc.o: qbby.h calc.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) calc.cpp 
gui.o: qbby.h qbbyFunc.h rsdl.h gui.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) gui.cpp
main.o: qbby.h qbbyFunc.h rsdl.h main.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) main.cpp
clean:
	rm *.o
