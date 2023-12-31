echo "Compiling files..."

g++ -c server.cpp -o server.o
g++ -c http_tcpserver.cpp -o http_tcpserver.o
g++ -std=c++20 -Wall server.o http_tcpserver.o -o server -lws2_32

echo "Finished compiling"
echo "Starting server..."

./server