#ifndef INCLUDED_HTTP_TCPSERVER
#define INCLUDED_HTTP_TCPSERVER

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string>

using namespace std;

namespace http
{
  class TcpServer
  {
    public:
      TcpServer(string ip_address, int port);
      ~TcpServer();
      void startListen();

    private:
      string m_ip_address;
      int m_port;
      string m_route;
      SOCKET m_socket;
      SOCKET m_new_socket;
      long m_incomingMessage;
      struct sockaddr_in m_socketAddress;
      int m_socketAddress_len;
      string m_serverMessage;
      WSADATA m_wsaData;

      int startServer();
      void closeServer();
      void acceptConnection(SOCKET& new_socket);
      void sendResponse();
      string buildResponse(char* buffer);
      string getHttpRoute(char* buffer);
      int getRouteStartingIndex(char* buffer, char startBreakChar);
      int getRouteEndingIndex(char* buffer, int routeStartIndex, char endBreakChar);
      string buildHtmlFile(string routeName);
  };

} // namespace http

#endif