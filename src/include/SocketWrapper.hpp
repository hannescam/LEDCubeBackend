#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <functional>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <thread>
#include <ctime>
#include <chrono>
#include "communicationDefinitions.hpp"

using namespace std;

class SocketHandler {
  public:
    void startListner();
    bool isOpen();
    unsigned long getConnectionId();
    bool sendString(string data);
    bool sendByteArray(const char* data, int size);
    string receiveStringUntilMessageEnd();
    char* receiveByteArry(int size);
    void setDisconnectHandler(function<void(SocketHandler*)> _disconnectHandler);
    void setReceiveHandler(function<void(string, SocketHandler*)> _receiveHandler);
    void setUseReveiveHandler(bool useHandler);
    void setPacketSize(int _packetSize);
    void setConnectionFileDescriptor(int _connfd);
  private:
    int connfd;
    int packetSize;
    unsigned long connectionId;
    bool isConnOpen = true;
    bool useHandler = true;
    thread listenerThread;
    thread stopHandlerThread;
    chrono::time_point<chrono::high_resolution_clock> lastKeepaliveRequest;

    void stopHandler();
    string keepaliveHandler(string input);
    void listener();
    function<void(string, SocketHandler*)> receiveHandler;
    function<void(SocketHandler*)> disconnectHandler;
};

class SocketAcceptor {
  public:
    bool begin(int port, int _packetSize = 64);
    void startListening();
    void stopListening();
    void closeSocket();
    void setClientHandler(function<void(SocketHandler*)> _clientHandler);
  private:
    int sockfd;
    int packetSize;
    bool stopListeningVar = false;

    void startClientHandler(SocketHandler* socketConnection);
    void listener();
    vector<thread> clientHandlerThreads;
    vector<SocketHandler> clientHandlers;

    thread listnerThread;
    function<void(SocketHandler*)> clientHandlerFunction;
};

