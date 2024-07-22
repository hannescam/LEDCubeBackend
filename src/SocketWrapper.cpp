#include "include/SocketWrapper.hpp"

void SocketAcceptor::startClientHandler(SocketHandler* socketConnection) {
  clientHandlerFunction(socketConnection);
}

void SocketAcceptor::listener() {
  int connfd;
  struct sockaddr_in cli;
  unsigned int cliLength;
  while (!stopListeningVar) {
    cliLength = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &cliLength);
    if (connfd < 0) {
      Logger::urgent("Failed to accept incoming connection request", LOG_AEREA_SOCKET_ACCEPTOR);
    } else {
      Logger::debug("Accepted incoming connection request", LOG_AEREA_SOCKET_ACCEPTOR);
      usleep(5000);

      clientHandlers.resize(clientHandlers.size() + 1);
      clientHandlers.at(clientHandlers.size() - 1).setPacketSize(packetSize);
      clientHandlers.at(clientHandlers.size() - 1).setConnectionFileDescriptor(ref(connfd));
      clientHandlers.at(clientHandlers.size() - 1).startListner();

      thread clientHandlerThread(&SocketAcceptor::startClientHandler, this, &clientHandlers.at(clientHandlers.size() - 1));
      //clientHandlerThread.detach();
      //while (clientHandlers.at(clientHandlers.size() - 1).isOpen()) usleep(50000); // Prevent multiple clients from joining at the same time
      clientHandlerThreads.push_back(move(clientHandlerThread));
    }
    usleep(1000);
  }
  stopListeningVar = false;
}

bool SocketAcceptor::begin(int port, int _packetSize) {
  packetSize = _packetSize;
  struct sockaddr_in servaddr;

  Logger::debug("Trying to create a C socket...", LOG_AEREA_SOCKET_ACCEPTOR);
  sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP/IP socket using the C socket api
  if (sockfd == -1) {
    Logger::error("Error creating socket", LOG_AEREA_SOCKET_ACCEPTOR);
    exit(1);
    return false;
  }

  Logger::debug("Successfully created socket", LOG_AEREA_SOCKET_ACCEPTOR);

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  Logger::debug("Trying to bind the socket to address/port", LOG_AEREA_SOCKET_ACCEPTOR);
  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
    Logger::error("Error binding socket to address/port", LOG_AEREA_SOCKET_ACCEPTOR);
    exit(1);
    return false;
  }

  Logger::debug("Successfully bound socket to address/port", LOG_AEREA_SOCKET_ACCEPTOR);
  Logger::debug("Trying to start listening on the socket...", LOG_AEREA_SOCKET_ACCEPTOR);
  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
    Logger::error("Error while trying to listen on the socket", LOG_AEREA_SOCKET_ACCEPTOR);
    exit(1);
    return false;
  }

  Logger::info("Started listening on socket", LOG_AEREA_SOCKET_ACCEPTOR);

  return true;
}

void SocketAcceptor::startListening() {
  Logger::debug("Starting listener thread...", LOG_AEREA_SOCKET_ACCEPTOR);

  thread _listenerThread(&SocketAcceptor::listener, this);
  _listenerThread.detach();
  listnerThread = move(_listenerThread);

  Logger::info("Successfully started listener thread", LOG_AEREA_SOCKET_ACCEPTOR);
}

void SocketAcceptor::stopListening() {
  stopListeningVar = true;
}

void SocketAcceptor::closeSocket() {
  Logger::info("Closing socket...", LOG_AEREA_SOCKET_ACCEPTOR);
  close(sockfd);
}

void SocketAcceptor::setClientHandler(function<void(SocketHandler*)> _clientHandler) {
  clientHandlerFunction = _clientHandler;
}
