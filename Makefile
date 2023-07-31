OPCIONS = -g -O0 -Wall -Wextra -Werror -Wno-implicit-fallthrough -ansi

all: main

main: main.o cambra.o laberint.o teseus.o dedalus.o
	g++ -o main main.o cambra.o laberint.o teseus.o dedalus.o -lesin
	rm *.o

main.o:	main.cpp
	g++ -c main.cpp $(OPCIONS)

cambra.o: cambra.cpp
	g++ -c cambra.cpp $(OPCIONS)

laberint.o: laberint.cpp
	g++ -c laberint.cpp $(OPCIONS)

teseus.o: teseus.cpp
	g++ -c teseus.cpp $(OPCIONS)

dedalus.o: dedalus.cpp
	g++ -c dedalus.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.gch