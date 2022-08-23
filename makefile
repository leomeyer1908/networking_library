prog: main.o ServerSocket.o
	g++ main.o ServerSocket.o -o server

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

ServerSocket.o: ServerSocket.cpp ServerSocket.hpp
	g++ -std=c++11 -c ServerSocket.cpp

clean:
	rm -f *.o server