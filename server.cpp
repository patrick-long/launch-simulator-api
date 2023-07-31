#include "./http_tcpserver.h"

int main()
{
  using namespace http;
  
  TcpServer testServer = TcpServer("127.0.0.1", 8080);
  testServer.startListen();
  
  return 0;
}