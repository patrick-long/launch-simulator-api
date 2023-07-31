FROM gcc:4.9

COPY . /usr/src/launch-simulator-api
WORKDIR /usr/src/launch-simulator-api

RUN mkdir -p build/ && cd build/ && rm -rf *

RUN echo "Compiling files..."
RUN cd build/ && gcc -I.. -o LaunchSimulatorLinux ../server_linux.cpp ../http_tcpserver_linux.cpp -lstdc++
RUN echo "Finished compiling"

EXPOSE 8080