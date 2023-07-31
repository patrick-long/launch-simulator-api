#include "./http_tcpserver_linux.h"

#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace
{
  const int BUFFER_SIZE = 30720;

  void log(const string& message)
  {
    cout << message << endl;
  }

  void exitWithError(const string& errorMessage)
  {
    log("ERROR: " + errorMessage);
    exit(1);
  }
}

namespace http
{
  TcpServer::TcpServer(string ip_address, int port)
    : m_ip_address(ip_address),
      m_port(port),
      m_socket(),
      m_new_socket(),
      m_incomingMessage(),
      m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress))
  {
    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if (startServer() != 0)
    {
      ostringstream ss;
      ss
        << "Failed to start server with PORT: "
        << ntohs(m_socketAddress.sin_port)
        << "\n";
        
      exitWithError(ss.str());
    }
  } // TcpServer::TcpServer()

  TcpServer::~TcpServer()
  {
    closeServer();
  } // TcpServer::~TcpServer()

  void TcpServer::startListen()
  {
    if (listen(m_socket, 20) < 0)
    {
      exitWithError("Socket listen failed.");
    }

    ostringstream ss;
    ss
      << "\n*** Listening on ADDRESS: "
      << inet_ntoa(m_socketAddress.sin_addr)
      << " PORT: "
      << ntohs(m_socketAddress.sin_port)
      << " ***\n";

    log(ss.str());

    int bytesReceived;
    while (true)
    {
      log("===== Waiting for a new connection =====\n\n");
      acceptConnection(m_new_socket);

      char buffer[BUFFER_SIZE] = { 0 };
      bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);

      if (bytesReceived < 0)
      {
        exitWithError("Failed to receive bytes from client socket connection.");
      }

      ostringstream ss;
      ss << "----- Received Request from client -----\n\n";
      log(ss.str());

      m_serverMessage = buildResponse(buffer);
      sendResponse();

      close(m_new_socket);
    }
  } // TcpServier::startListen()

  // PRIVATE METHODS
  int TcpServer::startServer()
  {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socket < 0)
    {
      exitWithError("Cannot create socket.");
      return 1;
    }

    if (
      bind(
        m_socket,
        (sockaddr*)&m_socketAddress,
        m_socketAddress_len
      ) < 0
    )
    {
      exitWithError("Cannot connect socket to address.");
      return 1;
    }

    return 0;
  } // TcpServer::startServer()

  void TcpServer::closeServer()
  {
    close(m_socket);
    close(m_new_socket);
    exit(0);
  } // TcpServer::closeServer()

  void TcpServer::acceptConnection(int& new_socket)
  {
    new_socket = accept(
      m_socket,
      (sockaddr*)&m_socketAddress,
      &m_socketAddress_len
    );

    if (new_socket < 0)
    {
      ostringstream ss;
      ss
        << "Server failed to accept incoming connection from ADDRESS: "
        << inet_ntoa(m_socketAddress.sin_addr)
        << "; PORT: "
        << ntohs(m_socketAddress.sin_port);

      exitWithError(ss.str());
    }
  } // TcpServer::acceptConnection()

  string TcpServer::buildResponse(char buffer[BUFFER_SIZE])
  {
    string routeName = getHttpRoute(buffer);
    string htmlFile = buildHtmlFile(routeName);

    ostringstream ss;
    ss
      << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
      << htmlFile.size()
      << "\n\n"
      << htmlFile;

    return ss.str();
  } // TcpServer::buildResponse()

  string TcpServer::getHttpRoute(char buffer[BUFFER_SIZE])
  {
    ostringstream route;

    char startBreakChar = '/';
    int routeStartIndex = getRouteStartingIndex(buffer, startBreakChar);

    char endBreakChar = ' ';
    int routeEndIndex = getRouteEndingIndex(buffer, routeStartIndex, endBreakChar);


    for (int i = routeStartIndex; i < routeEndIndex; i++)
    {
      route << buffer[i];
    }

    return route.str();
  } // TcpServer::getHttpRoute()

  int TcpServer::getRouteStartingIndex(char buffer[BUFFER_SIZE], char startBreakChar)
  {
    int result;

    for (int i = 0; i < 20; i++)
    {
      if (buffer[i] == startBreakChar)
      {
        result = i;
        break;
      }
    }

    return result;
  } // TcpServer::getRouteStartingIndex()

  int TcpServer::getRouteEndingIndex(
    char buffer[BUFFER_SIZE],
    int routeStartIndex,
    char endBreakChar
  )
  {
    int result;

    for (int i = routeStartIndex; i < BUFFER_SIZE; i++)
    {
      if (buffer[i] == endBreakChar)
      {
        result = i;
        break;
      }
    }

    return result;
  } // TcpServer::getRouteEndingIndex()

  string TcpServer::buildHtmlFile(string routeName)
  {
    ostringstream htmlFile;

    htmlFile
      << "<!DOCTYPE html><html lang=\"en\"><body><h1> Launch Simulator API "
      << routeName
      << " </h1><p> Hello from your Server :) </p><p>This is some more text in the response body</p></body></html>";

    return htmlFile.str();
  } // TcpServer::buildHtmlFile()

  void TcpServer::sendResponse()
  {
    long bytesSent;

    bytesSent = write(
      m_new_socket,
      m_serverMessage.c_str(),
      m_serverMessage.size()
    );

    if (bytesSent == m_serverMessage.size())
    {
      log("----- Server Response sent to client -----\n\n");
    }
    else
    {
      log("Error sending response to client.\n");
    }
  } // TcpServer::sendResponse()
} // namespace http
