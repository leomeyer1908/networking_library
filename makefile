prog: server.o ServerSocket.o
	g++ server.o ServerSocket.o -o server

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

ServerSocket.o: ServerSocket.cpp ServerSocket.hpp
	g++ -std=c++11 -c ServerSocket.cpp

clean:
	rm -f *.o server