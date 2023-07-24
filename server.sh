g++ -c server.cpp -o server.o
g++ -c http_tcpserver.cpp -o http_tcpserver.o
g++ -std=c++11 -Wall server.o http_tcpserver.o -o server -lws2_32
./server